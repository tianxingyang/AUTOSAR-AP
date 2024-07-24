#include "ara/log/logging_handler.h"

#include "ara/core/string_view.h"
#include "ara/log/dlt_message.h"
#include "fmt/core.h"


namespace ara::log {
void ConsoleHandler::Emit(std::shared_ptr<dlt::Message> msg) { fmt::print("{}\n", msg->ToString()); }
}  // namespace ara::log