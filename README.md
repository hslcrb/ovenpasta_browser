# Ovenpasta Browser

Ovenpasta는 외부 종속성 없이 오직 순수 C++와 GTK3만을 사용하여 처음부터 바닥부터 만들어진 매우 가볍고 교육적인 웹 브라우저입니다. HTTP 파싱부터 HTML DOM 트리 구성, 그리고 화면 렌더링에 이르기까지 필수적인 단계들을 이해할 목적으로 개발되었습니다.

## 주요 기능
*   **독자적인 네트워크 스택**: POSIX 소켓을 사용한 순수 HTTP/1.1 클라이언트 구현.
*   **독자적인 HTML 파서**: 처음부터 구현한 기초적인 HTML 토크나이저 및 DOM(Document Object Model) 트리 생성기.
*   **독자적인 렌더링 엔진**: 생성된 DOM 트리를 순회하며 GTK와 Cairo를 이용해 화면에 직접 텍스트를 그리는 시스템.
*   **GTK3 기반 UI**: 주소 표시줄, 뒤로가기 버튼, 스크롤 가능한 렌더링 영역을 포함한 직관적인 인터페이스.

## 파일 구조

```
ovenpasta_browser/
├── CMakeLists.txt        # 빌드 스크립트
├── .gitignore
├── src/
│   ├── main.cpp          # 애플리케이션 진입점
│   ├── core/             # 주요 엔진 연결부 (확장 예정)
│   ├── ui/               # GTK3 윈도우 및 이벤트 루프 처리
│   ├── network/          # POSIX HTTP GET 클라이언트
│   ├── html/             # HTML 파서 및 DOM 노드 구조체
│   └── render/           # Cairo 기반 DOM 트리 렌더러
```

## 빌드 및 실행 방법

### 요구 사항
* C++ 17 이상을 지원하는 컴파일러 (GCC/Clang)
* CMake (버전 3.10 이상)
* GTK3 개발 라이브러리 (`libgtk-3-dev`)

### 빌드하기
Ubuntu/Debian 환경 기준, 터미널을 열고 다음 명령어를 실행합니다:

```bash
# 1. 빌드 폴더 생성 및 이동
mkdir build && cd build

# 2. CMake 설정
cmake ..

# 3. 컴파일
make
```

### 실행하기
빌드가 완료되었다면 아래 명령어로 브라우저를 실행할 수 있습니다.
```bash
./ovenpasta
```
*   주소표시줄에 대상 URL을 입력하고 **Go** 버튼을 누르세요. 
*   **주의**: 외부 라이브러리 연동(OpenSSL 등)을 배제했으므로 `https://` 대신 `http://` 프로토콜만 사용할 수 있습니다.

## 기여하기
버그 수정, 성능 향상, 혹은 CSS 파서 구현 시도 등 어떠한 형태의 기여도 환영합니다! 기여를 원하신다면 [CONTRIBUTING.md](CONTRIBUTING.md) 파일을 확인해 주세요.

## 라이선스
이 프로젝트는 MIT 라이선스 하에 배포됩니다. 자세한 내용은 [LICENSE](LICENSE) 파일을 참고하세요.
