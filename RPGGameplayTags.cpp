#include "RPGGameplayTags.h"


namespace RPGGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG(Player_ChangeEquipment, "Player.ChangeEquipment");

	UE_DEFINE_GAMEPLAY_TAG(Ability_PlayerMovement, "Ability.PlayerMovement");
	
	UE_DEFINE_GAMEPLAY_TAG(Skill_DefaultAttack, "Skill.DefaultAttack");
	UE_DEFINE_GAMEPLAY_TAG(Skill_Dodge, "Skill.Dodge");
	UE_DEFINE_GAMEPLAY_TAG(Skill_TripleSlash, "Skill.TripleSlash");
	UE_DEFINE_GAMEPLAY_TAG(Skill_HurricaneKick, "Skill.HurricaneKick");
	
	UE_DEFINE_GAMEPLAY_TAG(Combat_QueryDamage, "Combat.QueryDamage");
	UE_DEFINE_GAMEPLAY_TAG(Combat_EnableNextCombo, "Combat.EnableNextCombo");
	UE_DEFINE_GAMEPLAY_TAG(Combat_SpawnProjectile, "Combat.SpawnProjectile");
	UE_DEFINE_GAMEPLAY_TAG(Combat_UpdateFacingTarget, "Combat.UpdateFacingTarget");

	UE_DEFINE_GAMEPLAY_TAG(State_HitReact, "State.HitReact");
	UE_DEFINE_GAMEPLAY_TAG(State_Dead, "State.Dead");
	
	UE_DEFINE_GAMEPLAY_TAG(Effect_Duration, "Effect.Duration");
	UE_DEFINE_GAMEPLAY_TAG(Effect_Percentage, "Effect.Percentage");

	UE_DEFINE_GAMEPLAY_TAG(NPCInteraction_StartInteraction, "NPCInteraction.StartInteraction");
	UE_DEFINE_GAMEPLAY_TAG(NPCInteraction_EndInteraction, "NPCInteraction.EndInteraction");

	UE_DEFINE_GAMEPLAY_TAG(Inventory_UpdateGold, "Inventory.UpdateGold");

	UE_DEFINE_GAMEPLAY_TAG(Quest_Action_Accept, "Quest.Action.Accept");
	UE_DEFINE_GAMEPLAY_TAG(Quest_Action_Drop, "Quest.Action.Drop");
	UE_DEFINE_GAMEPLAY_TAG(Quest_Action_Complete, "Quest.Action.Complete");
	UE_DEFINE_GAMEPLAY_TAG(Quest_ChangeState, "Quest.ChangeState");
	UE_DEFINE_GAMEPLAY_TAG(Quest_Notify_TaskSignal, "Quest.Notify.TaskSignal");
	
	UE_DEFINE_GAMEPLAY_TAG(Attribute_HP, "Attribute.HP");
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Exp, "Attribute.Exp");
	UE_DEFINE_GAMEPLAY_TAG(Attribute_MaxExp, "Attribute.MaxExp");
	
	UE_DEFINE_GAMEPLAY_TAG(Attribute_MaxHP_Base,	"Attribute.MaxHP.Base");
	UE_DEFINE_GAMEPLAY_TAG(Attribute_MaxHP_Flat,	"Attribute.MaxHP.Flat");
	UE_DEFINE_GAMEPLAY_TAG(Attribute_MaxHP_Percent, "Attribute.MaxHP.Percent");
	
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Attack_Base, 	 "Attribute.Attack.Base");
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Attack_Flat, 	 "Attribute.Attack.Flat");
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Attack_Percent, "Attribute.Attack.Percent");
	
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Defense_Base		,"Attribute.Defense.Base");
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Defense_Flat		,"Attribute.Defense.Flat");
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Defense_Percent	,"Attribute.Defense.Percent");

	UE_DEFINE_GAMEPLAY_TAG(Attribute_CriticalRate_Base, "Attribute.CriticalRate.Base");
	UE_DEFINE_GAMEPLAY_TAG(Attribute_CriticalRate_Flat, "Attribute.CriticalRate.Flat");
	UE_DEFINE_GAMEPLAY_TAG(Attribute_CriticalRate_Percent, "Attribute.CriticalRate.Percent");

	UE_DEFINE_GAMEPLAY_TAG(Attribute_MoveSpeed_Base, "Attribute.MoveSpeed.Base");
	UE_DEFINE_GAMEPLAY_TAG(Attribute_MoveSpeed_Flat, "Attribute.MoveSpeed.Flat");
	UE_DEFINE_GAMEPLAY_TAG(Attribute_MoveSpeed_Percent, "Attribute.MoveSpeed.Percent");

	UE_DEFINE_GAMEPLAY_TAG(Skill_Attribute_BaseDamage, "Skill.Attribute.BaseDamage");
	UE_DEFINE_GAMEPLAY_TAG(Skill_Attribute_AttackCoefficient, "Skill.Attribute.AttackCoefficient");
	UE_DEFINE_GAMEPLAY_TAG(Skill_Attribute_ChannelingTime, "Skill.Attribute.ChannelingTime");
	
	UE_DEFINE_GAMEPLAY_TAG(Widget_Inventory_UpdateSlotSize, "Widget.Inventory.UpdateSlotSize");
	UE_DEFINE_GAMEPLAY_TAG(Widget_Inventory_UpdateSlot, "Widget.Inventory.UpdateSlot");
	UE_DEFINE_GAMEPLAY_TAG(Widget_Inventory_UpdateTooltip, "Widget.Inventory.UpdateTooltip");
	
	UE_DEFINE_GAMEPLAY_TAG(Widget_Equipment_UpdateSlot, "Widget.Equipment.UpdateSlot");
	
	UE_DEFINE_GAMEPLAY_TAG(Widget_QuickBar_UpdateSlot, "Widget.QuickBar.UpdateSlot");
	UE_DEFINE_GAMEPLAY_TAG(Widget_QuickBar_UpdateCooldown, "Widget.QuickBar.UpdateCooldown");
	
	UE_DEFINE_GAMEPLAY_TAG(Widget_SkillTree_Initialize, "Widget.SkillTree.Initialize");
	UE_DEFINE_GAMEPLAY_TAG(Widget_SkillTree_SkillPointUp , "Widget.SkillTree.SkillPointUp");
	UE_DEFINE_GAMEPLAY_TAG(Widget_SkillTree_UpdateTooltip , "Widget.SkillTree.UpdateTooltip");
	
	UE_DEFINE_GAMEPLAY_TAG(Widget_Slot_Interaction, "Widget.Slot.Interaction");
	UE_DEFINE_GAMEPLAY_TAG(Widget_Slot_DragDrop, "Widget.Slot.DragDrop");
	
	UE_DEFINE_GAMEPLAY_TAG(Widget_Dialogue_UpdateDialogue, "Widget.Dialogue.UpdateDialogue")

	UE_DEFINE_GAMEPLAY_TAG(Widget_Quest_ShowNpcQuestInfo, "Widget.Quest.ShowNpcQuestInfo");
	UE_DEFINE_GAMEPLAY_TAG(Widget_Quest_UpdateNpcQuestList, "Widget.Quest.UpdateNpcQuestList");
	
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Attack_1, "Enemy.Attack.1");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Attack_2, "Enemy.Attack.2");
}
