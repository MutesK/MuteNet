param(
	[Parameter(Mandatory=$true)]
	[string]$SourcePath,

	[Parameter(Mandatory=$true)]
	[string]$TargetPath,

	[switch]$Recursive,
	[switch]$WhatIfMode = $true,
	[switch]$Overwrite
)

# 안전 확인
if (-not (Test-Path $SourcePath)) {
	Write-Error "Source path does not exist: $SourcePath"
	exit 1
}

if (-not (Test-Path $TargetPath)) {
	Write-Host "Target path does not exist. Creating: $TargetPath"
	New-Item -ItemType Directory -Path $TargetPath | Out-Null
}

Write-Host "Source : $SourcePath"
Write-Host "Target : $TargetPath"
Write-Host "Recursive: $Recursive, WhatIfMode: $WhatIfMode, Overwrite: $Overwrite"

$searchOption = if ($Recursive) { 'AllDirectories' } else { 'TopDirectoryOnly' }

# 수집: 원본 파일 목록
$sourceFiles = Get-ChildItem -Path $SourcePath -File -Recurse:($Recursive.IsPresent) -ErrorAction Stop

$toCopy = @()
$conflicts = @()

foreach ($f in $sourceFiles) {
	$relative = Resolve-Path $f.FullName | ForEach-Object { $_.Path.Substring((Resolve-Path $SourcePath).Path.Length).TrimStart('\\') }
	$dest = Join-Path $TargetPath $relative
	if (Test-Path $dest) {
		$conflicts += @{ Source=$f.FullName; Dest=$dest }
	}
	else {
		$toCopy += @{ Source=$f.FullName; Dest=$dest }
	}
}

Write-Host "\nSummary:"
Write-Host "  Total source files : $($sourceFiles.Count)"
Write-Host "  New files to copy  : $($toCopy.Count)"
Write-Host "  Conflicting files   : $($conflicts.Count)"

if ($toCopy.Count -gt 0) {
	Write-Host "\nFiles that will be copied (new):"
	$toCopy | Select-Object -First 20 | ForEach-Object { Write-Host "  $_.Source -> $_.Dest" }
	if ($toCopy.Count -gt 20) { Write-Host "  ...and $($toCopy.Count - 20) more" }
}

if ($conflicts.Count -gt 0) {
	Write-Host "\nConflicting files (already exist in target, will NOT be overwritten by default):"
	$conflicts | Select-Object -First 20 | ForEach-Object { Write-Host "  $_.Source -> $_.Dest" }
	if ($conflicts.Count -gt 20) { Write-Host "  ...and $($conflicts.Count - 20) more" }
}

if ($WhatIfMode) {
	Write-Host "\n-- WhatIf mode: no files will be copied. Use -WhatIfMode:$false to perform actual copy."
	exit 0
}

Write-Host "\nStarting copy..."

# 실제 복사
foreach ($item in $toCopy) {
	$destDir = Split-Path $item.Dest -Parent
	if (-not (Test-Path $destDir)) { New-Item -ItemType Directory -Path $destDir | Out-Null }
	Copy-Item -Path $item.Source -Destination $item.Dest -Force:$Overwrite
	Write-Host "Copied: $($item.Source) -> $($item.Dest)"
}

if ($conflicts.Count -gt 0 -and $Overwrite) {
	Write-Host "\nOverwriting conflicting files as requested..."
	foreach ($c in $conflicts) {
		Copy-Item -Path $c.Source -Destination $c.Dest -Force
		Write-Host "Overwritten: $($c.Source) -> $($c.Dest)"
	}
}

Write-Host "\nDone."
