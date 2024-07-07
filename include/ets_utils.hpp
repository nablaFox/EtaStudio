#pragma once

#include "ets_pch.hpp"

namespace etsutil {

struct DeletionQueue {
	std::deque<std::function<void()>> deletors;

	void push_function(std::function<void()>&& deletor) { deletors.push_back(deletor); }

	void flush() {
		for (auto it = deletors.rbegin(); it != deletors.rend(); it++)
			(*it)();

		deletors.clear();
	}
};

}; // namespace etsutil
