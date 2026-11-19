from pathlib import Path
import hashlib
import subprocess
import sys

APPLY = "--apply" in sys.argv
moves = []
deletes = []
errors = []

def git(*args):
    return subprocess.run(["git", *map(str, args)], check=True)

def sha256(p):
    h = hashlib.sha256()
    with p.open("rb") as f:
        for chunk in iter(lambda: f.read(1024 * 1024), b""):
            h.update(chunk)
    return h.hexdigest()

pairs = [
    (Path("CO/CO_2023"), Path("CO/CO 2023")),
    (Path("CO/CO_2024"), Path("CO/CO 2024")),
]

tracked = set(
    subprocess.check_output(
        ["git", "ls-files", "-z"]
    ).decode().split("\0")
)

# Preflight CO consolidation
for src, dst in pairs:
    if not src.exists():
        continue

    for p in sorted(src.rglob("*")):
        if not p.is_file():
            continue

        if p.as_posix() not in tracked:
            errors.append(f"UNTRACKED SOURCE: {p}")
            continue

        target = dst / p.relative_to(src)

        if target.exists():
            if target.is_file() and sha256(p) == sha256(target):
                deletes.append(p)
            else:
                errors.append(f"CONFLICT: {p} -> {target}")
        else:
            moves.append((p, target))

# Preflight original repository archive
original = Path("resources")
archive = Path("archive/source-repositories")

if original.exists():
    if archive.exists():
        errors.append(f"ARCHIVE ALREADY EXISTS: {archive}")
    else:
        for p in original.rglob("*"):
            if p.is_file() and p.as_posix() not in tracked:
                errors.append(f"UNTRACKED IMPORT FILE: {p}")

# Check Git status, allowing preexisting modified audit reports
status = subprocess.check_output(
    ["git", "status", "--porcelain", "-z"]
).decode().split("\0")

for entry in filter(None, status):
    path = entry[3:]
    if (
        not path.startswith("reports/")
        and not path.startswith("scripts/final_structure_cleanup.py")
    ):
        errors.append(f"UNEXPECTED WORKTREE CHANGE: {entry}")

print("\n=== FINAL STRUCTURAL CLEANUP ===")
print("CO files to move:", len(moves))
print("Identical CO files to remove:", len(deletes))
print("Archive old resources:", original.exists())

for p, target in moves:
    print("MOVE:", p, "->", target)

for p in deletes:
    print("REMOVE IDENTICAL:", p)

if errors:
    print("\nBLOCKERS:")
    for e in errors:
        print(" ", e)
    print("\nNo changes applied.")
    sys.exit(1)

if not APPLY:
    print("\nDRY RUN COMPLETE. No files modified.")
    print("Run with --apply to execute.")
    sys.exit(0)

# Move unique CO files
for p, target in moves:
    target.parent.mkdir(parents=True, exist_ok=True)
    git("mv", "--", p, target)

# Remove only identical files
for p in deletes:
    git("rm", "--", p)

# Archive source repository metadata
if original.exists():
    archive.parent.mkdir(parents=True, exist_ok=True)
    git("mv", "--", original, archive)

print("\nCLEANUP APPLIED.")
print("Review staged changes before committing.")
