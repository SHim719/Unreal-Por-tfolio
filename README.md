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

- 스킬 트리 UI를  블루프린트에서 직접 배치하지 않고, DataAsset에 정의된 행과 열 정보를 기반으로 자동 배치되도록 구현했습니다.
- 스킬 간의 연결선 역시 블루프린트 상에서 수동으로 그리는 방식이 아닌, NativePaint() 함수에서 스킬들의 연결 관계를 읽어와 자동으로 선을 그려주는 구조로 구성했습니다.
- 이로 인해 스킬 트리 구조가 변경되더라도 UI를 블루프린트에서 일일이 수정할 필요 없이, 데이터 수정만으로 자동 반영될 수 있어서 효과적이고 빠르게 스킬트리를 UI를 만들 수 있었습니다.

## 퀘스트
<img src="https://github.com/SHim719/Image/blob/main/Quest%EA%B5%AC%EC%A1%B0.png" alt="이미지" width="700" height="600">
<img src="https://github.com/SHim719/Image/blob/main/QuestPrerequirements.png" alt="이미지" width="700">
<img src="https://github.com/SHim719/Image/blob/main/QuestTask.png" alt="이미지" width="700">


  

  





 
 
