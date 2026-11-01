
#!/usr/bin/env python3

import hashlib
import csv
from collections import defaultdict
from pathlib import Path

ROOT = Path(".").resolve()
REPORTS = ROOT / "reports"

REPORTS.mkdir(exist_ok=True)

# Directories that should not be included in the audit.
EXCLUDED_DIRS = {
    ".git",
    ".codespaces",
    "__pycache__",
    "node_modules",
    "venv",
    ".venv",
    "reports",
}

def sha256_file(path):
    """Return the SHA-256 hash of a file."""
    digest = hashlib.sha256()

    with path.open("rb") as f:
        while True:
            chunk = f.read(1024 * 1024)

            if not chunk:
                break

            digest.update(chunk)

    return digest.hexdigest()


def get_files():
    """Recursively collect repository files."""
    files = []

    for path in ROOT.rglob("*"):
        if not path.is_file():
            continue

        relative = path.relative_to(ROOT)

        if any(part in EXCLUDED_DIRS for part in relative.parts):
            continue

        files.append(path)

    return files


files = get_files()

print(f"Files found: {len(files)}")
print("Calculating hashes...")

# Group files by hash.
hash_groups = defaultdict(list)

# Group files by filename.
name_groups = defaultdict(list)

for path in files:
    relative = path.relative_to(ROOT).as_posix()

    file_hash = sha256_file(path)

    hash_groups[file_hash].append(relative)

    name_groups[path.name].append(
        (relative, file_hash)
    )


# ---------------------------------------------------
# REPORT 1: Exact duplicate files
# ---------------------------------------------------

exact_duplicates = []

for file_hash, paths in hash_groups.items():

    if len(paths) > 1:

        exact_duplicates.append({
            "sha256": file_hash,
            "count": len(paths),
            "files": paths,
        })


with (REPORTS / "exact_duplicates.csv").open(
    "w",
    newline="",
    encoding="utf-8"
) as f:

    writer = csv.writer(f)

    writer.writerow([
        "SHA256",
        "Number of copies",
        "File paths",
    ])

    for group in exact_duplicates:

        writer.writerow([
            group["sha256"],
            group["count"],
            " | ".join(group["files"]),
        ])


# ---------------------------------------------------
# REPORT 2: Same filename, different contents
# ---------------------------------------------------

different_content = []

for filename, entries in name_groups.items():

    unique_hashes = {
        file_hash
        for _, file_hash in entries
    }

    if len(entries) > 1 and len(unique_hashes) > 1:

        different_content.append({
            "filename": filename,
            "files": entries,
        })


with (REPORTS / "same_name_different_content.csv").open(
    "w",
    newline="",
    encoding="utf-8"
) as f:

    writer = csv.writer(f)

    writer.writerow([
        "Filename",
        "SHA256",
        "File path",
    ])

    for group in different_content:

        for path, file_hash in group["files"]:

            writer.writerow([
                group["filename"],
                file_hash,
                path,
            ])


# ---------------------------------------------------
# REPORT 3: Summary
# ---------------------------------------------------

print()
print("Audit complete.")
print()

print(
    "Exact duplicate groups:",
    len(exact_duplicates)
)

print(
    "Same-name/different-content groups:",
    len(different_content)
)

print()
print("Reports generated:")
print("reports/exact_duplicates.csv")
print("reports/same_name_different_content.csv")
