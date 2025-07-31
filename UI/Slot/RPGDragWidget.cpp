


#include "UI/Slot/RPGDragWidget.h"

#include "Components/Image.h"

void URPGDragWidget::SetIcon(UTexture2D* IconTexture)
{
	Image_Icon->SetBrushFromTexture(IconTexture);
	
}
