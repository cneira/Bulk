#pragma once

#include <cstddef>
#include <cstring>
#include <memory>
#include <tuple>
#include <vector>

#include "world.hpp"
#include "util/meta_helpers.hpp"

/**
 * \file messages.hpp
 *
 * This header defines message-passing support.
 */

namespace bulk {

using namespace bulk::meta;

// queue::impl subclasses queue_base
// The reason that this is seperate is:
// queue::impl has template Tag,Content
// whereas we need to access this object
// in a virtual function in `world`.
class queue_base {
  public:
    queue_base(){};
    virtual ~queue_base(){};

    // These are called by world during a sync
    // It resizes an internal buffer and returns a pointer to it
    virtual void* get_buffer_(int size_in_bytes) = 0;
    virtual void clear_() = 0;

    virtual void unsafe_push_back(void* msg) = 0;
    virtual void unsafe_push_array(int count, size_t size, void* msg,
                                   size_t size_of_other, void* other) = 0;
};

/**
 * The queue inferface is used for sending and receiving messages.
 *
 * \tparam Tag the type to use for the message tag
 * \tparam Content the type to use for the message content
 */
template <typename T, typename... Ts>
class queue {
  public:
    using message_type = decltype(message<T, Ts...>::content);
    using iterator = typename std::vector<message_type>::iterator;

    // If T = E[] is an array type, then element_type is E, otherwise it is T
    using element_type =
        typename std::remove_pointer<typename std::decay<T>::type>::type;

    /**
     * A queue is a mailbox for messages of a given type.
     * They allow for a convenient message passing syntax:
     *
     *     q(processor).send(content...);
     */
    class sender {
      public:
        /** Send multiple messages over the queue. */
        void send(std::vector<message_type> msgs) {
            // FIXME implement this as a single internal primitive
            for (auto msg : msgs) {
                q_.impl_->send_(t_, msg);
            }
        }

        /** Send a message over the queue. */
        template <typename... Us>
        void send(Us... args) {
            message_type msg = {args...};
            q_.impl_->send_(t_, msg);
        }

        /* Enabled if first content type is an array. */
        // We have to let this depend on its own template parameter to allow
        // SFINAE to kick in
        template <typename U = T,
                  typename = typename std::enable_if_t<
                      std::is_array<U>::value && (sizeof...(Ts) == 0)>>
        void send_many(std::vector<element_type> msgs) {
            q_.impl_->send_many_(t_, msgs.size(), msgs.data(), nullptr, 0);
        }

        template <typename U = T,
                  typename = typename std::enable_if_t<
                      std::is_array<U>::value && (sizeof...(Ts) > 0)>>
        void send_many(std::vector<element_type> msgs, Ts... args,
                       void* = nullptr) {
            message_type msg = {std::vector<element_type>{}, args...};
            q_.impl_->send_many_(t_, msgs.size(), msgs.data(), &msg,
                                 sizeof(message_type));
        }

      private:
        friend queue;

        sender(queue& q, int t) : q_(q), t_(t) {}

        queue& q_;
        int t_;
    };

    /**
     * Construct a message queue and register it with world
     * The world implementation can choose to perform a synchronization
     */
    queue(bulk::world& world) { impl_ = std::make_unique<impl>(world); }
    ~queue() {}

    // Disallow copies
    queue(queue& other) = delete;
    void operator=(queue& other) = delete;

    /**
      * Move a queue.
      */
    queue(queue&& other) { impl_ = std::move(other.impl_); }

    /**
     * Move a queue.
     */
    void operator=(queue&& other) { impl_ = std::move(other.impl_); }

    /**
     * Get an object with which you can send to a remote queue
     */
    auto operator()(int t) { return sender(*this, t); }

    /**
     * Get an iterator to the begin of the local queue
     */
    iterator begin() { return impl_->data_.begin(); }

    /**
     * Get an iterator to the end of the local queue
     */
    iterator end() { return impl_->data_.end(); }

    /**
     * Get the number of messages in the local queue.
     */
    size_t size() { return impl_->data_.size(); }

    /**
     * Check if the queue is empty.
     */
    bool empty() { return impl_->data_.empty(); }

    /**
     * Get a reference to the world of the queue.
     *
     * \returns a reference to the world of the queue
     */
    bulk::world& world() { return impl_->world_; }

  private:
    class impl : public queue_base {
      public:
        impl(bulk::world& world) : world_(world) {
            id_ = world.register_queue_(this);
        }
        ~impl() { world_.unregister_queue_(id_); }

        // No copies or moves
        impl(impl& other) = delete;
        impl(impl&& other) = delete;
        void operator=(impl& other) = delete;
        void operator=(impl&& other) = delete;

        void send_(int t, message_type m) {
            world_.send_(t, id_, &m, sizeof(m));
        }

        void send_many_(int t, int count, element_type* m, void* other,
                        size_t size_of_other) {
            world_.send_many_(t, id_, m, sizeof(element_type), count, other,
                              size_of_other);
        }

        void* get_buffer_(int size_in_bytes) override {
            data_.resize(size_in_bytes / sizeof(message_type));
            return &data_[0];
        }

        void unsafe_push_back(void* msg) override {
            data_.push_back(*static_cast<message_type*>(msg));
        }

        void unsafe_push_array(int count, size_t size, void* data,
                               size_t size_of_other, void* other) override {
            if (size_of_other == 0) {
                data_.push_back(message_type{});
                auto& msg =
                    *((std::vector<element_type>*)(&(data_[data_.size() - 1])));
                msg.resize(count);
                memcpy(msg.data(), data, size * count);
            } else {
                data_.push_back(*static_cast<message_type*>(other));
                using real_type = std::tuple<std::vector<element_type>, Ts...>;
                auto& msg = *((std::vector<element_type>*)(&(
                    std::get<0>(*(real_type*)&data_[data_.size() - 1]))));
                msg.resize(count);
                memcpy(msg.data(), data, size * count);
            }
        }

        void clear_() override { data_.clear(); }

        std::vector<message_type> data_;
        bulk::world& world_;
        int id_;
    };
    std::unique_ptr<impl> impl_;

    friend sender;
};

} // namespace bulk
