
#!/usr/bin/env python3

import csv
import hashlib
import re
from collections import defaultdict
from pathlib import Path

ROOT = Path(".").resolve()
REPORTS = ROOT / "reports"

REPORTS.mkdir(exist_ok=True)

EXCLUDED_DIRS = {
    ".git",
    ".codespaces",
    "__pycache__",
    "node_modules",
    "venv",
    ".venv",
    "reports",
}

YEAR_PATTERN = re.compile(r"20\d{2}")


def sha256_file(path):
    digest = hashlib.sha256()

    with path.open("rb") as f:
        while True:
            chunk = f.read(1024 * 1024)

            if not chunk:
                break

            digest.update(chunk)

    return digest.hexdigest()


def get_subject_year(relative_path):
    """
    Extract subject and year from a file path.

    Examples:
        OS/OS 2022/exams/midsem.pdf
        OS/OS-A 2025/exams/midsem.pdf
        OS/OS (Sec B) 2025/exams/midsem.pdf
    """

    parts = relative_path.parts

    if len(parts) < 2:
        return None

    subject = parts[0]

    year = None

    for part in parts[1:]:

        match = YEAR_PATTERN.search(part)

        if match:
            year = match.group()

            break

    if year is None:
        return None

    return subject, year


def get_files():

    files = []

    for path in ROOT.rglob("*"):

        if not path.is_file():
            continue

        relative = path.relative_to(ROOT)

        if any(
            part in EXCLUDED_DIRS
            for part in relative.parts
        ):
            continue

        files.append(path)

    return files


# Group files by subject, year, and filename.
groups = defaultdict(list)

print("Scanning files...")

for path in get_files():

    relative = path.relative_to(ROOT)

    subject_year = get_subject_year(relative)

    if subject_year is None:
        continue

    subject, year = subject_year

    filename = path.name

    file_hash = sha256_file(path)

    key = (subject, year, filename)

    groups[key].append({
        "path": relative.as_posix(),
        "hash": file_hash,
    })


# Generate the report.
report_path = (
    REPORTS /
    "subject_year_duplicate_candidates.csv"
)

duplicate_groups = 0
different_content_groups = 0

with report_path.open(
    "w",
    newline="",
    encoding="utf-8"
) as f:

    writer = csv.writer(f)

    writer.writerow([
        "Subject",
        "Year",
        "Filename",
        "SHA256",
        "File path",
        "Duplicate status",
    ])

    for (subject, year, filename), entries in sorted(
        groups.items()
    ):

        if len(entries) < 2:
            continue

        hashes = {
            entry["hash"]
            for entry in entries
        }

        if len(hashes) == 1:

            status = "EXACT_DUPLICATE"

            duplicate_groups += 1

        else:

            status = "SAME_NAME_DIFFERENT_CONTENT"

            different_content_groups += 1

        for entry in entries:

            writer.writerow([
                subject,
                year,
                filename,
                entry["hash"],
                entry["path"],
                status,
            ])


print()
print("Audit complete.")
print()
print("Exact duplicate groups:", duplicate_groups)
print(
    "Same-name/different-content groups:",
    different_content_groups
)

print()
print("Report:")
print(report_path.relative_to(ROOT))
