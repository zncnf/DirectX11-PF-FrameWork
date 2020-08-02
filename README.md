# DirectX11-PF-FrameWork
DirectX11 PF FrameWork

기본적인 프로젝트 셋팅
 - 미리컴파일된 헤더파일 사용합니다 - D3DUtil.h
 - 유니코드 문자 집합 사용합니다.
 - 경로문제나 라이브러리 포함할수 없는 에러발생시 포함 디렉터리, 라이브디렉터리, 추가 포함 디렉터리가 재대로 설정되있는지 확인해주세요.
 - 안보이는 소스파일이 있을경우 솔루션 탐색기에서 모든 파일 표시 눌러주세요 모든소스파일은 Include 폴더내에 있습니다.
 - Text출력은 Direct2D와 DirectWrite를 사용합니다. (Image는 아직 미정)
 - 코드가 긴부분이나 가독성이 떨어지는 부분은 region 기능으로 나눴습니다.. region이 보이면 접어서 어떤 기능을 하는 코드인지 확인하세요

DirectX 초기화 부분까지 완료된 프레임워크입니다. 프로그램 최초 실행시 왼쪽 상단에 "Text Sample 1" 이라는 문구가 떠야됩니다.
