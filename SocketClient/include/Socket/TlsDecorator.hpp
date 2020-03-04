//
// Created by NB22477 on 04/03/2020.
//

#ifndef TLSDECORATOR_HPP
#define TLSDECORATOR_HPP

#include <functional>
#include <tls.h>
#include <memory>
#include <string>
#include <vector>
#include <mutex>
#include <condition_variable>

class TlsDecorator {

public:

    bool Connect(const std::string& serverName) {

        this->tlsConfig = decltype(tlsConfig)(
            tls_config_new(),
            [](tls_config* p) {
                tls_config_free(p);
            }
        );

        this->tlsImpl = decltype(tlsImpl)(
            tls_client(),
            [](tls* p) {
                tls_close(p);
                tls_free(p);
            }
        );

        return (tls_connect_cbs(
                this->tlsImpl.get(),
                [](struct tls *_ctx, void *_buf, size_t _buflen, void *_cb_arg) {
                    const auto self = (TlsDecorator*) _cb_arg;
                    std::lock_guard<decltype(self->mutex)> lock(self->mutex);

                    const auto amt = std::min(_buflen, self->receiveBuffer.size());

                    if (amt == 0 && self->open) {
                        return (ssize_t)TLS_WANT_POLLIN;
                    }

                    return (ssize_t) amt;
                },
                [](struct tls *_ctx, const void *_buf, size_t _buflen, void *_cb_arg) {
                    const auto self = (TlsDecorator*) _cb_arg;
                    std::lock_guard<decltype(self->mutex)> lock(self->mutex);

                    const auto bufBytes = (const uint8_t*)_buf;

                    self->SendData(std::vector<uint8_t>(bufBytes, bufBytes + _buflen));

                    return (ssize_t) _buflen;
                },
                this,
                serverName.data()
                ) == 0);
    }

    void SendData(const std::vector<uint8_t>& data) {
        std::unique_lock<decltype(mutex)> lock(mutex);
        size_t offset = 0;

        while (offset < data.size()) {

            const auto amount = tls_write(this->tlsImpl.get(), data.data(), data.size());

            if (amount == TLS_WANT_POLLIN) {
                wakeCondition.wait(
                        lock,
                        [this] {
                            return !open || !receiveBuffer.empty();
                        }
                );
            }else if (amount < 0) {
                // stop connection
            } else {
                offset += amount;
            }
        }


    }

private:

    std::vector<uint8_t> receiveBuffer;

    std::unique_ptr<tls, std::function<void(tls*)>> tlsImpl;

    std::unique_ptr<tls_config, std::function<void(tls_config*)>> tlsConfig;

    std::mutex mutex;

    std::condition_variable wakeCondition;

    bool open = true;
};

#endif //TLSDECORATOR_HPP
