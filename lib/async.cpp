#include "async.h"

#include "handler.h"

namespace async {


handle_t connect(std::size_t bulk) {
    return new Handler(bulk);
}

void receive(handle_t handle, const char *data, std::size_t size) {
    if(handle) {
        static_cast<Handler*>(handle)->receive(data, size);
    }
}

void disconnect(handle_t handle) {
    if (handle) {
        delete static_cast<Handler*>(handle);
    }
}

} // namespace async
