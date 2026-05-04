param(
    [string]$Root = "F:\minix-vm-os"
)

$required = @("nasm", "i686-elf-gcc", "grub-mkrescue", "qemu-system-i386")
$missing = @()

foreach ($tool in $required) {
    $cmd = Get-Command $tool -ErrorAction SilentlyContinue
    if (-not $cmd) {
        $missing += $tool
    }
}

if ($missing.Count -gt 0) {
    Write-Host "Missing tools: $($missing -join ', ')"
    exit 1
}

Write-Host "Toolchain check passed for $Root"
exit 0
