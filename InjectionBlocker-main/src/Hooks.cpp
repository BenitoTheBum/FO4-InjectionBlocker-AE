#include "Hooks.h"
#undef AddForm

namespace Hooks
{
	void LeveledItem_AddForm(RE::TESLevItem& a_self, RE::TESForm* a_item, std::int32_t a_level, std::int32_t a_count)
	{
		if (!a_item) {
			return;
		}

		if (iniData.whitelist.find(a_item) != iniData.whitelist.end()) {
			logger::warn("{:08X} is a whitelisted item", a_item->formID);
			a_self.AddScriptAddedLeveledObject(&a_self, static_cast<std::uint16_t>(a_level), static_cast<std::uint16_t>(a_count), a_item);
			return;
		}

		if (iniData.bBlockAllItems) {
			logger::warn("Blocked an item injection: {:08X}", a_item->formID);
			return;
		}

		if (iniData.bBlockAmmo && a_item->formType == RE::ENUM_FORM_ID::kAMMO) {
			logger::warn("Blocked an ammo injection: {:08X}", a_item->formID);
			return;
		}

		if (iniData.bBlockArmor && a_item->formType == RE::ENUM_FORM_ID::kARMO) {
			logger::warn("Blocked an armor injection: {:08X}", a_item->formID);
			return;
		}

		if (iniData.bBlockWeapons && a_item->formType == RE::ENUM_FORM_ID::kWEAP) {
			logger::warn("Blocked a weapon injection: {:08X}", a_item->formID);
			return;
		}

		a_self.AddScriptAddedLeveledObject(&a_self, static_cast<std::uint16_t>(a_level), static_cast<std::uint16_t>(a_count), a_item);
	}

	void LeveledActor_AddForm(RE::TESLevCharacter& a_self, RE::TESForm* a_actor, std::int32_t a_level)
	{
		if (!a_actor) {
			return;
		}

		if (iniData.whitelist.find(a_actor) != iniData.whitelist.end()) {
			logger::warn("{:08X} is a whitelisted actor", a_actor->formID);
			a_self.AddScriptAddedLeveledObject(&a_self, static_cast<std::uint16_t>(a_level), 1, a_actor);
			return;
		}

		if (iniData.bBlockNPCs && a_actor->formType == RE::ENUM_FORM_ID::kNPC_) {
			logger::warn("Blocked an NPC injection: {:08X}", a_actor->formID);
			return;
		}

		a_self.AddScriptAddedLeveledObject(&a_self, static_cast<std::uint16_t>(a_level), 1, a_actor);
	}

	bool RegisterPapyrus(RE::BSScript::IVirtualMachine* a_vm)
	{
		if (!a_vm) {
			return false;
		}

		a_vm->BindNativeMethod("LeveledItem", "AddForm", LeveledItem_AddForm, true);
		a_vm->BindNativeMethod("LeveledActor", "AddForm", LeveledActor_AddForm, true);
		logger::info("Registered runtime-independent Papyrus AddForm hooks");
		return true;
	}

	void Install()
	{
		if (const auto papyrus = F4SE::GetPapyrusInterface()) {
			papyrus->Register(RegisterPapyrus);
		} else {
			logger::error("Failed to get F4SE Papyrus interface");
		}
	}
}
