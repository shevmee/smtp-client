#pragma once

namespace ISXResponse {
enum class StatusType {
  PositiveCompletion,
  PositiveIntermediate,
  TransientNegative,
  PermanentNegative,
  Undefined
};
}  // namespace ISXResponse