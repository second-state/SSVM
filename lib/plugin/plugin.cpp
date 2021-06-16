// SPDX-License-Identifier: Apache-2.0
#include "plugin/plugin.h"
#include "common/defines.h"
#include "common/version.h"

#if defined(WASMEDGE_OS_LINUX)
#include <pwd.h>
#include <unistd.h>
#endif

namespace WasmEdge {
namespace Plugin {

Expect<Plugin> Plugin::load(const std::filesystem::path &Path) noexcept {
  auto Lib = std::make_shared<Loader::SharedLibrary>();
  if (auto Res = Lib->load(Path); unlikely(!Res)) {
    return Unexpect(Res);
  }

  Plugin P{Path, std::move(Lib)};
  if (auto Func =
          P.Lib->get<decltype(wasmedgeGetPluginInfo)>("wasmedgeGetPluginInfo");
      unlikely(!Func)) {
    return Unexpect(ErrCode::WrongInstanceAddress);
  } else {
    P.Info = Func();
  }

  if (P.Info.APIVersion != WASMEDGE_PLUGIN_API_VERSION) {
    return P;
  }

  return P;
}

std::vector<std::filesystem::path>
Plugin::enumerate(const std::filesystem::path &SearchPath) {
  using namespace std::literals;
  std::vector<std::filesystem::path> Result;
  std::error_code Error;
  if (std::filesystem::is_directory(SearchPath, Error)) {
    for (auto Iter :
         std::filesystem::recursive_directory_iterator(SearchPath)) {
      const auto &Path = Iter.path();
      if (Path.extension().u8string() == ".so"sv) {
        Result.push_back(Iter.path());
      }
    }
  }

  auto GlobalPluginDir = std::filesystem::u8path(kGlobalPluginDir);
#if defined(WASMEDGE_OS_LINUX) || defined(WASMEDGE_OS_MACOS)
  if (std::filesystem::is_directory(GlobalPluginDir, Error)) {
    for (auto Iter : std::filesystem::directory_iterator(kGlobalPluginDir)) {
      const auto &Path = Iter.path();
      if (Path.extension().u8string() == ".so"sv) {
        Result.push_back(Iter.path());
      }
    }
  }
  std::filesystem::path Home;
  if (auto HomeEnv = getenv("HOME")) {
    Home = std::filesystem::u8path(HomeEnv);
  } else {
    auto Passwd = getpwuid(getuid());
    Home = std::filesystem::u8path(Passwd->pw_dir);
  }
  auto LocalPluginDir = Home / ".wasmedge"sv / "plugins"sv;
  if (std::filesystem::is_directory(LocalPluginDir, Error)) {
    for (auto Iter : std::filesystem::directory_iterator(LocalPluginDir)) {
      const auto &Path = Iter.path();
      if (Path.extension().u8string() == ".so"sv) {
        Result.push_back(Iter.path());
      }
    }
  }
#elif defined(WASMEDGE_OS_WINDOWS)
  if (std::filesystem::is_directory(GlobalPluginDir, Error)) {
    for (auto Iter : std::filesystem::directory_iterator(kGlobalPluginDir)) {
      const auto &Path = Iter.path();
      if (Path.extension().u8string() == ".dll"sv) {
        Result.push_back(Iter.path());
      }
    }
  }
#endif
  return Result;
}

} // namespace Plugin
} // namespace WasmEdge
