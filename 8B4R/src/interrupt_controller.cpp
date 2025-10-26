#include "interrupt_controller.hpp"

InterruptController::InterruptController() : irq_pending(false), irq_vector(0) {}

void InterruptController::request(uint8_t vector) {
    irq_pending = true;
    irq_vector = vector;
}

void InterruptController::clear() {
    irq_pending = false;
}

bool InterruptController::is_pending() const {
    return irq_pending;
}