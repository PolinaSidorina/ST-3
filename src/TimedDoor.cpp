// Copyright 2025 PolinaSidorina

#include "TimedDoor.h"
#include <stdexcept>
#include <thread>
#include <chrono>
#include <future>

DoorTimerAdapter::DoorTimerAdapter(TimedDoor& d) : door(d) {}

void DoorTimerAdapter::Timeout() {
    if (door.isDoorOpened()) {
        door.throwState();
    }
}

TimedDoor::TimedDoor(int timeout) : iTimeout(timeout), isOpened(false) {
    adapter = new DoorTimerAdapter(*this);
}

bool TimedDoor::isDoorOpened() {
    return isOpened;
}

void TimedDoor::unlock() {
    isOpened = true;
    auto timeoutFuture = std::async([this, timeout]() {
        std::this_thread::sleep_for(std::chrono::seconds(iTimeout));
        adapter->Timeout();
    });
}

void TimedDoor::lock() {
    isOpened = false;
}

int TimedDoor::getTimeOut() const {
    return iTimeout;
}

void TimedDoor::throwState() {
    if (isOpened) {
        throw std::runtime_error("Door left open too long!");
    }
}

void Timer::sleep(int seconds) {
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
}

void Timer::tregister(int timeout, TimerClient* client) {
    std::thread([timeout, client](){
        std::this_thread::sleep_for(std::chrono::seconds(timeout));
        client->Timeout();
    }).detach();
}
