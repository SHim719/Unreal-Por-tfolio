<div align="center">
  
# 탑 다운 싱글 RPG 프로젝트(Unreal Engine 5, GAS, 1人)
<img src="https://github.com/SHim719/Image/blob/main/%EC%96%B8%EB%A6%AC%EC%96%BCRPG%ED%91%9C%EC%A7%80.png" alt="이미지" width="500">

</div>

# 동영상
https://www.youtube.com/watch?v=QNZ20ZTuVFo

# 프로젝트 구현사항

## 랜덤 아이템 옵션
<img src="https://github.com/SHim719/Image/blob/main/%EA%B2%801.png" alt="이미지" width="200"><img src="https://github.com/SHim719/Image/blob/main/%EA%B2%802.png" alt="이미지" width="200"><img src="https://github.com/SHim719/Image/blob/main/%EA%B2%803.png" alt="이미지" width="200">
<img src="https://github.com/SHim719/Image/blob/main/ItemOptionFragment.png" alt="이미지" width="500" height ="600">
<img src="https://github.com/SHim719/Image/blob/main/%EC%95%84%EC%9D%B4%ED%85%9C%EC%98%B5%EC%85%98%EC%85%94%ED%94%8C.png" alt="이미지" width="500" height ="600">
<img src="https://github.com/SHim719/Image/blob/main/%EC%95%84%EC%9D%B4%ED%85%9C%EC%98%B5%EC%85%98%EC%84%B8%ED%8C%85.png" alt="이미지" width="800" height ="800">


파밍의 재미를 위한 랜덤 장비 아이템 옵션을 구현했습니다.
- 아이템 옵션들은 DataTable로 관리되며, GameplayEffect를 활용해 각각의 효과를 재사용 가능하고 독립적으로 정의할 수 있도록 구성합니다.
- GameplayTag를 키 값으로 사용해 아이템 옵션의 수치 범위를 지정합니다.
- 아이템 생성 시 이 범위 내에서 랜덤한 수치가 결정되어, 동일한 장비라도 다양한 옵션 구성이 가능하도록 설계했습니다.


## 아이템 옵션의 동작
<img src="https://github.com/SHim719/Image/blob/main/%EC%95%84%EC%9D%B4%ED%85%9C%EC%98%B5%EC%85%98%EB%8F%99%EC%9E%91.png" alt="이미지" width="600" height ="400">
<img src="https://github.com/SHim719/Image/blob/main/SentToASC.png" alt="이미지" width="600" height ="400">

- 아이템 옵션은 즉시 스탯이 변경되는(공격력 증가, 최대 HP 증가) 옵션과 이벤트가 발생했을 때(공격, 피격 등), 확률적으로 발동하는 옵션 두 가지로 나뉩니다.
- 즉시 스탯이 변경되는 옵션은 장비 장착 시 즉시 적용합니다.
- 확률 옵션들의 관리는 ProcComponent가 이벤트 수신과 관리를 담당합니다. 이벤트 발생 시, 확률 계산 후 옵션을 ProcEffect에 명시되어있는 대상에게 적용합니다.

 ## 스킬
<img src="https://github.com/SHim719/Image/blob/main/ProjectileAbility.png" alt="이미지" width="600" height ="800"><img src="https://github.com/SHim719/Image/blob/main/MeleeAbility.png" alt="이미지" width="400" height ="700">
 <img src="https://github.com/SHim719/Image/blob/main/Ability%EA%B5%AC%EC%A1%B0.png" alt="이미지" width="700" height ="500">

- 스킬들은 GameplayAbility를 상속해서 큰 분류의 스킬 클래스를 만들고 이를 상속하여 파라미터를 조절하는 방식으로 구현했습니다. 
- 근접 스킬 같은 경우는 애니메이션과 타격 횟수 파라미터를 조정하고, Projectile이나 AOE 스킬은 Actor Class만 바꿔줌으로써, 다양한 스킬을 쉽게 정의할 수 있습니다. 
- CurveTable을 이용해, 스킬 레벨에 따른 스킬의 속성(데미지, AOE 지속 시간, 쿨타임 등)들을 세팅해주었습니다.

## 스킬 트리
<img src="https://github.com/SHim719/Image/blob/main/%EC%8A%A4%ED%82%AC%ED%8A%B8%EB%A6%AC%EC%8A%A4%ED%81%AC%EB%A6%B0%EC%83%B7.png" alt="이미지" width="500" height ="400">
<img src="https://github.com/SHim719/Image/blob/main/%EC%8A%A4%ED%82%AC%ED%8A%B8%EB%A6%AC%EC%BD%94%EB%93%9C1png.png" alt="이미지">

- 스킬 트리 UI를 블루프린트에서 직접 배치하지 않고, DataAsset에 정의된 행과 열 정보를 기반으로 자동 배치되도록 구현했습니다.
- 스킬 간의 연결선 역시 블루프린트 상에서 수동으로 그리는 방식이 아닌, NativePaint() 함수에서 스킬들의 연결 관계를 읽어와 자동으로 선을 그려주는 구조로 구성했습니다.
- 이로 인해 스킬 트리 구조가 변경되더라도 UI를 블루프린트에서 일일이 수정할 필요 없이, 데이터 수정만으로 자동 반영될 수 있어서 효과적이고 빠르게 스킬트리를 UI를 만들 수 있었습니다.

## 퀘스트
<img src="https://github.com/SHim719/Image/blob/main/Quest%EA%B5%AC%EC%A1%B0.png" alt="이미지" width="700" height="600">
<img src="https://github.com/SHim719/Image/blob/main/QuestTask.png" alt="이미지" width="700">

- 퀘스트의 Task(목표)와 Prerequirement(요구사항)는 상속 가능한 베이스 클래스를 기반으로 설계하여, 다양한 퀘스트 유형을 유연하게 확장할 수 있도록 구성했습니다. 
- 이를 통해 "몬스터 처치", "아이템 수집" 같은 퀘스트 목표를 모듈식으로 재사용하고, 퀘스트 조건 또한 독립적으로 조합할 수 있습니다.

  
## 퀘스트 시스템의 동작
<img src="https://github.com/SHim719/Image/blob/main/%ED%80%98%EC%8A%A4%ED%8A%B8UML.png" alt="이미지" width="500" >

- 퀘스트 시스템은 NPC, QuestManager, UI 간의 메시지 전파 구조를 기반으로 설계했습니다.
- 플레이어가 대화를 통해 퀘스트를 수락하거나 포기하면 QuestManager는 해당 변경 메시지를 전파하고, UI와 NPC는 이를 수신해 업데이트합니다.
- 또한, 전투나 아이템 습득 같은 인게임 이벤트 발생 시 QuestManager가 모든 진행 중 퀘스트의 Task를 검사하고 조건 달성 여부를 판단하여, 조건이 충족되면 완료 메시지를 다시 전파합니다.


## 오브젝트 풀
<img src="https://github.com/SHim719/Image/blob/main/%EC%98%A4%EB%B8%8C%EC%A0%9D%ED%8A%B8%ED%92%801.png" alt="이미지" height="400">
<img src="https://github.com/SHim719/Image/blob/main/%EC%98%A4%EB%B8%8C%EC%A0%9D%ED%8A%B8%ED%92%80%EC%BD%94%EB%93%9C2.png" alt="이미지" height="400">

- 투사체, 스킬 오브젝트, 데미지 숫자 UI 등 자주 반복 생성/삭제되는 객체의 생성/삭제의 오버헤드를 최소화하기 위해 오브젝트 풀을 구현했습니다.

## SceneCaputureComponent 최적화
<img src="https://github.com/SHim719/Image/blob/main/SC%EC%B5%9C%EC%A0%81%ED%99%94%EC%A0%84.png" alt="이미지">
<img src="https://github.com/SHim719/Image/blob/main/SC%EB%82%AE%EC%9D%80%ED%94%84%EB%A0%88%EC%9E%84.png" alt="이미지">
<img src="https://github.com/SHim719/Image/blob/main/SC%EC%B5%9C%EC%A0%81%ED%99%94%ED%9B%84.png" alt="이미지">
<img src="https://github.com/SHim719/Image/blob/main/SC%EB%86%92%EC%9D%80%ED%94%84%EB%A0%88%EC%9E%84.png" alt="이미지">

- 프레임이 낮아서 UnrealInsights로 프로파일링을 해보니 인벤토리에 플레이어를 띄우는 SceneCaptureComponent가 큰 부하를 주는 것을 확인했습니다.
- SceneCaptureComponent가 인벤토리를 열지 않았을 때에도 동작하고 있었고, 매 프레임 렌더링하고 있어서 프레임 드랍이 생긴다는 것을 발견했습니다.
- 인벤토리를 열때만 플레이어를 Capture하도록 세팅하고, Tick 주기를 0.1초정도로 조정하고 PlayerCaptureCharacter의 Tick 함수에서 SceneCaptureComponent의 Capture를 수동으로 호출하여 렌더링 프레임을 조절하였습니다.








  

  





 
 
