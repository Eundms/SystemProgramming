# SystemProgramming

- SIC/XE 어셈블러 구현 : SIC/XE 소스 Object Program Code로 변환

## 과제1
### 1a. C
- ControlSection 방식의 SIC/XE 소스를 Object Program Code로 바꾸는 어셈블러 만들기
- SIC/XE 소스를 라인별로 처리해서 Object Code로 바꾼 후, Object Program Code로 변환하는 프로그램

### 1b. Java
- 동일한 기능을 Java 버전으로 재구현

## 과제2 
- Control Section 기반의 SIC/XE Object Code를 Java GUI 기반 시뮬레이터로 실행 및 시각화
- Object Code를 메모리에 로드하고, 레지스터/메모리/Instruction 상태를 시각적으로 보여주며 Step-by-step 실행

## 관련 개념 

### Relocation, Linking, Packing
1) Relocation
임의의 주소에 오브젝트 프로그램을 올릴 때, 달라진 주소에 대해 오브젝트 프로그램을 수정하는 작업이다.  
2) Linking
여러 오브젝트 프로그램을 하나로 만들어주는 작업이다. ESTAB을 이용한다.  
3) Packing
오브젝트 프로그램에서 어셈블리된 코드는 16진수 char 형식으로 주어진다.  
그래서 이것을 실행시키기 위해 load하기 위해서는 1byte로 만들어야 한다.  
java에서는 c와 다르게 char는 2byte이다.  

### Absolute Loader
Linking과 Relocation을 진행하지 않아도 되는 프로그램의 Loader이다.  
H: 프로그램의 이름, 프로그램의 크기 
E; E레코드를 만나면 로더는 실행하기 위해 특정 주소 값으로 이동한다.   

### Linking Loader  
pass1에서는 모든 external symbol에 주소를 할당한다.  
pass2에서는 loading, relcation, linking을 진행한다. 


## 배운점
> 메모리에 올라간 데이터가 '명령어'인지 '값'인지 CPU는 어떻게 구분하지?
- CPU는 구분하지 않음
    - PC가 가리키는 위치를 명령어로 가정하고 디코딩하여 실행함
- 명령어는 구조가 정해져 있기 때문에 앞부분(opcode + 포맷 정보)를 해석하면 instruction 전체 크기를 알 수 있음 
    - SIC/XE 명령어 구조에서 
        - 1-6bits : opcode
        - 7-8bits : n, i -> addressing 방식
        - 9-12bits : x, b, p, e -> format 정보
        - 나머지 : 나머지 disp or address

1. 메모리에 있는 게 명령어인지 값인지 어떻게 알아요?	
- CPU는 모름, 우리는 opcode와 format으로 구분
- 실행 위치(PC)가 메모리 해석의 기준

2. 명령어 길이는 어떻게 알아요?	
- e bit + opcode table을 통해 format 확인

3. 값과 코드가 섞여있으면 안 헷갈리나요?	
- PC만 명령어 위치를 가리키기 때문에 문제 없음

## 개념
### 컴파일러
> 고급 언어를 어셈블리어 또는 중간 코드로 변역
- 단계
    - Lexical Analysis (어휘 분석): 토큰화 (변수, 키워드, 기호 등 구분)
    - Syntax Analysis (구문 분석): 문법 규칙에 맞는지 확인 (Parse Tree 생성)
    - Semantic Analysis (의미 분석): 변수 선언 여부, 타입 검사 등
    - Intermediate Code Generation (중간 코드 생성): 기계에 독립적인 코드 생성
    - Optimization (최적화): 성능 향상을 위한 코드 최적화
    - Code Generation: 어셈블리 언어로 변환하거나 바로 오브젝트 코드 생성

### 어셈블러
> 어셈블리 소스 코드를 받아서 기계어(Object Code)로 변환하는 프로그램
- Two-Pass Assembler 
    - Pass1 (기호 처리)
        1. Label, Opcode, Operand 분석
        2. Location Counter를 사용해 주소 계산
        3. SYMTAB 생성 : Label -> 주소
        4. LITTAB 및 Control Secion 처리 (SIC/XE 기준)
    - Pass2 (코드 생성)
        1. SYMTAB을 참조해 주소 결정
        2. 명령어를 Object Code로 변환
        3. Header(H), Text(T), Modification(M), End(E)레코드 생성

### 링커 
>  여러개의 Object 파일을 연결하여 하나의 실행 가능한 프로그램을 만드는 프로그램

- 외부 참조(EXTREF)를 연결
- 정의된 심볼(EXTDEF)을 찾아서 실제 주소를 바인딩
- 서로 다른 파일에 흩어진 코드/데이터를 한 공간에 모음
- Relocation 정보(M 레코드) 기반으로 주소 재조정

### 로더
> Object Code를 메모리에 올려 실행할 수 있게 준비하는 프로그램

- .obj 파일을 읽어서 메모리의 지정된 위치에 올림
- 시작 주소(프로그램 실행 시작점) 설정
- 필요 시 재배치 수행 (재배치 = 주소 수정)

