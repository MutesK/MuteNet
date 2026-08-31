사용법 요약 - migrate_files.ps1

1) 드라이런(시뮬레이션)으로 원본 검사
   powershell -NoProfile -ExecutionPolicy Bypass -File tools\migrate_files.ps1 -SourcePath "D:\\Workspace\\마이그레이션 목표 코드" -TargetPath "migration_source" -Recursive -WhatIfMode

2) 실제 복사(충돌 시 덮어쓰기 안함)
   powershell -NoProfile -ExecutionPolicy Bypass -File tools\migrate_files.ps1 -SourcePath "D:\\Workspace\\마이그레이션 목표 코드" -TargetPath "migration_source" -Recursive -WhatIfMode:$false

3) 충돌 시 덮어쓰기하려면 -Overwrite 스위치 추가

4) 복사 후 Visual Studio에서 파일을 프로젝트에 추가하려면
   - 솔루션 탐색기에서 프로젝트 우클릭 -> Add -> Existing Item...
   - migration_source 폴더에서 원하는 파일 선택
   - 다수 파일을 선택하려면 Ctrl/Shift 사용

5) 자동으로 .vcxproj에 추가하려면(수동 권장)
   - .vcxproj 파일은 XML이므로 수동으로 편집 가능하나, 잘못 편집 시 손상 위험
   - 안전하게 추가하려면 Visual Studio UI 사용
