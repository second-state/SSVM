// SPDX-License-Identifier: Apache-2.0
#include "plugin/plugin.h"
#include "po/list.h"
#include "po/option.h"
#include "processfunc.h"
#include "processmodule.h"

namespace WasmEdge {
namespace Host {
namespace {

PO::List<std::string> AllowCmd(
    PO::Description(
        "Allow commands called from wasmedge_process host functions. Each "
        "command can be specified as --allow-command `COMMAND`."sv),
    PO::MetaVar("COMMANDS"sv));
PO::Option<PO::Toggle> AllowCmdAll(PO::Description(
    "Allow all commands called from wasmedge_process host functions."sv));

void registerParser(PO::ArgumentParser &Parser) noexcept {
  Parser.add_option("allow-command"sv, AllowCmd)
      .add_option("allow-command-all"sv, AllowCmdAll);
}

Runtime::ImportObject *allocateHostModule(void) noexcept {
  return new WasmEdgeProcessModule;
}

} // namespace
} // namespace Host
} // namespace WasmEdge

WASMEDGE_SYMBOL_EXPORT ::WasmEdge::Plugin::PluginInfo wasmedgeGetPluginInfo() {
  return {
      .APIVersion = WASMEDGE_PLUGIN_API_VERSION,
      .PluginName = "wasmedge_process",
      .PluginVersion = "0.0.1",
      .RegisterParser = &registerParser,
      .AllocateHostModule = &allocateHostModule,
  };
}
