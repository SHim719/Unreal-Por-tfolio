#include "RPGEquipmentActor.h"


ARPGEquipmentActor::ARPGEquipmentActor()
{
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("MeshComp");
	SetRootComponent(Mesh);
}

void ARPGEquipmentActor::SetMesh(USkeletalMesh* InMesh)
{
	Mesh->SetSkeletalMesh(InMesh);
}
