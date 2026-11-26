#include "ConfigParser.h"
#include "Hooks.h"

CSimpleIniA simpleIni(true, false, false);
ModConfigs iniData;

F4SE_PLUGIN_VERSION = []() noexcept {
	F4SE::PluginVersionData v{};
	v.PluginVersion({ Version::MAJOR, Version::MINOR, Version::PATCH, 0 });
	v.PluginName(Version::PROJECT.data());
	v.AuthorName("GELUXRUM");
	v.UsesAddressLibrary(true);
	v.UsesAddressLibraryAE(true);
	v.UsesAddressLibraryNG(true);
	v.UsesSigScanning(false);
	v.IsLayoutDependent(true);
	v.IsLayoutDependentAE(true);
	v.IsLayoutDependentNG(true);
	v.CompatibleVersions({
		F4SE::RUNTIME_1_11_137,
		F4SE::RUNTIME_1_11_159,
		F4SE::RUNTIME_1_11_169,
		F4SE::RUNTIME_1_11_191,
		F4SE::RUNTIME_1_11_221,
		F4SE::RUNTIME_1_11_240,
		F4SE::RUNTIME_LATEST
	});
	return v;
}();

extern "C" DLLEXPORT bool F4SEAPI F4SEPlugin_Load(const F4SE::LoadInterface* a_f4se)
{
	F4SE::Init(a_f4se);

	logger::info("InjectionBlocker v{} by GELUXRUM", Version::NAME);

	Hooks::Install();

	const F4SE::MessagingInterface* messageInterface = F4SE::GetMessagingInterface();
	if (messageInterface) {
		messageInterface->RegisterListener([](F4SE::MessagingInterface::Message* msg) -> void {
			if (msg && msg->type == F4SE::MessagingInterface::kGameDataReady) {
				iniData.LoadConfigs(simpleIni);
			}
		});
	}

	return true;
}
