<div align="center">
  
# 쿼터뷰 싱글 RPG 프로젝트(개인 프로젝트)
<img src="https://github.com/SHim719/Image/blob/main/%EC%96%B8%EB%A6%AC%EC%96%BCRPG%ED%91%9C%EC%A7%80.png" alt="이미지" width="500">

**기술 스택: C++, Unreal Engine 5.5.4, Gameplay Ability System, Github Desktop**

</div>

# 동영상
https://www.youtube.com/watch?v=QNZ20ZTuVFo

# 프로젝트 구현사항

## GAS 기반 Data-Driven 아이템 옵션 시스템
<img src="https://github.com/SHim719/Image/blob/main/%EA%B2%801.png" alt="이미지" width="200"><img src="https://github.com/SHim719/Image/blob/main/%EA%B2%802.png" alt="이미지" width="200">
<img src="https://github.com/SHim719/Image/blob/main/ItemOptionFragment.png" alt="이미지" width="400" height ="500">
<img src="https://github.com/SHim719/Image/blob/main/%EC%95%84%EC%9D%B4%ED%85%9C%EA%B5%AC%EC%A1%B0.png" alt="이미지" height="600">

- **데이터 기반 아이템 생성**
  - DataTable과 GameplayTag를 활용해 옵션의 종류와 수치 범위를 정의하고, 코드 수정 없이 데이터만으로 아이템 다양성을 확장할 수 있는 구조를 구축했습니다.
  - 아이템 생성 시 데이터로 정의된 아이템 옵션 수치 범위 내에서 랜덤한 수치가 결정되어, 동일한 장비라도 다양한 옵션 구성이 가능하도록 설계했습니다.

<img src="https://github.com/SHim719/Image/blob/main/%EC%95%84%EC%9D%B4%ED%85%9C%EC%98%B5%EC%85%98%EB%8F%99%EC%9E%91.png" alt="이미지" width="600" height ="400">

- **Proc(확률 발동) 시스템 아키텍처**
  - 아이템 옵션은 즉시 스탯이 변경되는(공격력 증가, 최대 HP 증가) 옵션과 이벤트가 발생했을 때(공격, 피격 등), 확률적으로 발동하는 옵션 두 가지로 나뉩니다.
  - 공격/피격 등 특정 이벤트 발생 시, 확률 발동 옵션을 처리하기 위해 ProcComponent를 설계했습니다.
  - 즉시 적용되는 스탯(Passive)과 조건부 발동 효과(Active)의 처리 로직을 분리하여, 복잡한 전투 로직 안에서도 아이템 효과가 독립적으로 동작하도록 결합도를 낮췄습니다.
 
## SceneCaputureComponent 최적화
<img src="https://github.com/SHim719/Image/blob/main/SC%EC%B5%9C%EC%A0%81%ED%99%94%EC%A0%84.png" alt="이미지">
<img src="https://github.com/SHim719/Image/blob/main/SC%EB%82%AE%EC%9D%80%ED%94%84%EB%A0%88%EC%9E%84.png" alt="이미지">
<img src="https://github.com/SHim719/Image/blob/main/SC%EC%B5%9C%EC%A0%81%ED%99%94%ED%9B%84.png" alt="이미지">
<img src="https://github.com/SHim719/Image/blob/main/SC%EB%86%92%EC%9D%80%ED%94%84%EB%A0%88%EC%9E%84.png" alt="이미지">

- **문제**: UnrealInsights로 프로파일링을 해보니 인벤토리에 플레이어를 띄우는 SceneCaptureComponent가 큰 부하를 주는 것을 확인했습니다.
- **원인**: SceneCaptureComponent가 인벤토리를 열지 않았을 때에도 동작하고 있었고, 매 프레임 렌더링하고 있어서 프레임 드랍이 생긴다는 것을 발견했습니다.
- **해결** 인벤토리를 열때만 SceneCaputureComponent가 동작하도록 세팅하고, PlayerCaptureCharacter의 Tick 주기를 0.1초정도로 조정하고 Tick 함수에서 SceneCaptureComponent의 Capture를 수동으로 호출하여 렌더링 프레임을 조절하였습니다.

## 데이터 기반 스킬 트리와 UI 자동화
<img src="https://github.com/SHim719/Image/blob/main/%EC%8A%A4%ED%82%AC%ED%8A%B8%EB%A6%AC%EC%8A%A4%ED%81%AC%EB%A6%B0%EC%83%B7.png" alt="이미지" width="500" height ="400">
<img src="https://github.com/SHim719/Image/blob/main/%EC%8A%A4%ED%82%AC%ED%8A%B8%EB%A6%AC%EC%BD%94%EB%93%9C2.png" alt="이미지", height="400">

- 위젯을 수동으로 배치하는 하드코딩 방식을 지양하고, DataAsset에 정의된 그리드(Row, Column) 데이터를 기반으로 스킬이 자동 배치되도록 구현했습니다.
- 스킬 간의 연결선 역시 블루프린트 상에서 수동으로 배치하는 방식이 아닌, NativePaint() 함수에서 스킬들의 연결 관계를 읽어와 자동으로 선을 그려주는 구조로 구성했습니다.
- 이로 인해 스킬 트리 구조가 변경되더라도 UI를 블루프린트에서 일일이 수정할 필요 없이, 데이터 수정만으로 자동 반영될 수 있어서 효과적이고 빠르게 스킬트리를 UI를 만들 수 있었습니다.

<br>

## 퀘스트
<img src="https://github.com/SHim719/Image/blob/main/Quest%EA%B5%AC%EC%A1%B0.png" alt="퀘스트 구조" width="700" height="600">
<img src="https://github.com/SHim719/Image/blob/main/QuestTask.png" alt="퀘스트 태스크" width="700">

- 퀘스트의 Task(목표)와 Prerequirement(요구사항)는 상속 가능한 베이스 클래스를 기반으로 설계하여, 다양한 퀘스트 유형을 유연하게 확장할 수 있도록 구성했습니다.
- 이를 통해 "몬스터 처치", "아이템 수집" 같은 퀘스트 목표를 모듈식으로 재사용하고, 퀘스트 조건 또한 독립적으로 조합할 수 있습니다.

<br>

## 퀘스트 시스템의 동작
<img src="https://github.com/SHim719/Image/blob/main/%ED%80%98%EC%8A%A4%ED%8A%B8UML.png" alt="퀘스트 UML" width="500">

- 퀘스트 시스템은 NPC, QuestManager, UI 간의 메시지 전파 구조를 기반으로 설계했습니다.
- 플레이어가 대화를 통해 퀘스트를 수락하거나 포기하면 QuestManager는 해당 변경 메시지를 전파하고, UI와 NPC는 이를 수신해 업데이트합니다.
- 또한, 전투나 아이템 습득 같은 인게임 이벤트 발생 시 QuestManager가 모든 진행 중 퀘스트의 Task를 검사하고 조건 달성 여부를 판단하여, 조건이 충족되면 완료 메시지를 다시 전파합니다.
