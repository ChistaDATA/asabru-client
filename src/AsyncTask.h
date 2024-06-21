//
// Created by sinsinan on 13/06/24.
//

#ifndef CHISTA_ASABRU_ASYNC_TASK_H
#define CHISTA_ASABRU_ASYNC_TASK_H

#include <random>
#include <shared_mutex>
#include <string>
#include <vector>
#include <thread>
#include "async_api/packet.h"
#include "CommandDispatcher.h"

using namespace std;

typedef struct {
	char id[255];
	char action[255];
	string response;
	int retries;
	bool executed;
} ASYNC_TASK;

typedef vector<ASYNC_TASK> ASYNC_TASKS;

ASYNC_TASKS asyncTasks{};
shared_mutex asyncTasksMutex;
ASYNC_TASKS executedAsyncTasks{};
shared_mutex executedAsyncTasksMutex;

std::string sendAsyncAPIRequest(std::string packet) {
	ComputationContext ctx;
	ctx.Put("host", std::string("localhost"));
	ctx.Put("port",  8081);
	ctx.Put("serialized_request_packet", packet);

	if (!CommandDispatcher::Dispatch("AsyncAPIRequestCommand", &ctx)) {
		std::cout << "Failed to dispatch Async API request command.";
	}

	return ctx.GetString("serialized_response_packet");
}

int addAsyncTask(ASYNC_TASK asyncTask) {

	auto pkt = makeConnectPacket(asyncTask.action);
	auto serializedRespPacket = sendAsyncAPIRequest(serializeConnectPacket(&pkt));

	CONNECT_RESPONSE_PACKET* response_packet = (CONNECT_RESPONSE_PACKET *) serializedRespPacket.c_str();

	// TODO add null check
	memcpy(asyncTask.id, response_packet->task_id, 255);

	asyncTasksMutex.lock();
	asyncTasks.emplace_back(asyncTask);
	asyncTasksMutex.unlock();
	return 0;
}

bool asyncTaskStatus(ASYNC_TASK &asyncTask) {
	auto pkt = makeCheckStatusPacket(asyncTask.id);
	auto serializedRespPacket = sendAsyncAPIRequest(serializeCheckStatusPacket(&pkt));
	CHECK_STATUS_RESPONSE_PACKET* response_packet = (CHECK_STATUS_RESPONSE_PACKET *) serializedRespPacket.c_str();

	if (strcmp(response_packet->status, "complete"))
	{
		asyncTask.executed = true;
		asyncTask.response = response_packet->response;
		return true;
	}

	return false;
		
}

void checkAsyncTasks() {
	asyncTasksMutex.lock();
	for (ASYNC_TASKS::iterator it = asyncTasks.begin(); it != asyncTasks.end();) {
		if (asyncTaskStatus(*it)) {
			executedAsyncTasksMutex.lock();
			executedAsyncTasks.emplace_back(*it);
			executedAsyncTasksMutex.unlock();
			it = asyncTasks.erase(it);
		} else {
			it->retries += 1;
			++it;
		}
	}
	asyncTasksMutex.unlock();
}

void printCompletedAsyncTasks() {
	executedAsyncTasksMutex.lock();
	if (executedAsyncTasks.size() > 0) {
		std::cout << "=========================" << std::endl
				  << "  EXECUTED ASYNC TASKS   " << std::endl
				  << "=========================" << std::endl;
		for (ASYNC_TASKS::iterator i = executedAsyncTasks.begin(); i != executedAsyncTasks.end();) {
			std::cout << "|  id    | : " << i->id << std::endl
					  << "|response| : " << i->response << std::endl
					  << "-------------------------" << std::endl;
			i = executedAsyncTasks.erase(i);
		}
		std::cout << "=========================" << std::endl;
	}
	executedAsyncTasksMutex.unlock();
}

std::atomic<bool> asyncTaskThreadRunning(false);
std::thread asyncTaskThread;

void startAsyncTaskListeningThread() {
	if (!asyncTaskThreadRunning.load()) {
		asyncTaskThreadRunning.store(true);
		std::cout << "starting async tasks listening thread" << std::endl;
		asyncTaskThread = std::thread([] {
			while (asyncTaskThreadRunning.load()) {
				checkAsyncTasks();
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}
		});
	}
}

void stopAsyncTaskListeningThread() {
    asyncTaskThreadRunning.store(false);
	if (asyncTaskThread.joinable())
	{
		asyncTaskThread.join();
	}
}

#endif // CHISTA_ASABRU_ASYNC_TASK_H
