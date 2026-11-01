
#!/usr/bin/env python3

import hashlib
import csv
import shutil

from pathlib import Path

ROOT = Path(".").resolve()

OS_ROOT = ROOT / "OS"

DEST_ROOT = OS_ROOT / "OS 2025"

CONFLICT_ROOT = DEST_ROOT / "_CONFLICTS"

REPORTS = ROOT / "reports"

REPORTS.mkdir(exist_ok=True)

# Source folder -> destination folder.
FOLDER_MAP = {
    "OS-A 2025": "OS 2025 SecA",
    "OS-B 2025": "OS 2025 SecB",
    "OS (Sec B) 2025": "OS 2025 SecB",
}


def sha256(path):

    digest = hashlib.sha256()

    with path.open("rb") as f:

        for chunk in iter(
            lambda: f.read(1024 * 1024),
            b""
        ):

            digest.update(chunk)

    return digest.hexdigest()


def merge_folder(source, destination, conflicts):

    if not source.exists():

        print(f"WARNING: Source missing: {source}")

        return

    print(f"\nMerging: {source}")
    print(f"Into:    {destination}")

    for path in source.rglob("*"):

        if not path.is_file():

            continue

        relative = path.relative_to(source)

        target = destination / relative

        # New file: copy it.
        if not target.exists():

            target.parent.mkdir(
                parents=True,
                exist_ok=True
            )

            shutil.copy2(path, target)

            print(f"COPIED: {relative}")

            continue

        # Existing file: compare hashes.
        source_hash = sha256(path)

        target_hash = sha256(target)

        if source_hash == target_hash:

            print(f"IDENTICAL: {relative}")

            continue

        # Different contents: preserve both.
        conflict_target = (
            CONFLICT_ROOT /
            source.name /
            relative
        )

        conflict_target.parent.mkdir(
            parents=True,
            exist_ok=True
        )

        shutil.copy2(
            path,
            conflict_target
        )

        conflicts.append({
            "source": str(path.relative_to(ROOT)),
            "destination": str(target.relative_to(ROOT)),
            "conflict_copy": str(
                conflict_target.relative_to(ROOT)
            ),
            "source_sha256": source_hash,
            "destination_sha256": target_hash,
        })

        print(f"CONFLICT: {relative}")

        print(
            f"  Preserved at: {conflict_target}"
        )


def main():

    DEST_ROOT.mkdir(
        parents=True,
        exist_ok=True
    )

    conflicts = []

    for source_name, destination_name in FOLDER_MAP.items():

        source = OS_ROOT / source_name

        destination = (
            DEST_ROOT /
            destination_name
        )

        merge_folder(
            source,
            destination,
            conflicts
        )

    # Write conflict report.
    report = (
        REPORTS /
        "os_2025_conflicts.csv"
    )

    with report.open(
        "w",
        newline="",
        encoding="utf-8"
    ) as f:

        writer = csv.DictWriter(
            f,
            fieldnames=[
                "source",
                "destination",
                "conflict_copy",
                "source_sha256",
                "destination_sha256",
            ]
        )

        writer.writeheader()

        writer.writerows(conflicts)

    print("\n" + "=" * 60)

    print("OS 2025 consolidation complete.")

    print(
        "Conflicts found:",
        len(conflicts)
    )

    print(
        "Conflict report:",
        report.relative_to(ROOT)
    )

    print(
        "Original folders have NOT been deleted."
    )


if __name__ == "__main__":

    main()
