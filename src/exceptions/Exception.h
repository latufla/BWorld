#pragma once
#include <memory>
#include "../SharedHeaders.h"

#define EXCEPTION_INFO __FUNCTION__,__LINE__

namespace bw {
	class Exception : public std::exception {
	public:
		Exception(std::string func, uint32_t line, std::string reason) : func(func), line(line), reason(reason) {}
		~Exception() = default;

		virtual std::string msg() const {
			return func + "(" + std::to_string(line) + "): " + what() + " " + reason;
		};

	protected:
		std::string func;
		uint32_t line;
		std::string reason;
	};

	class InvalidObjectIdException : public Exception {
	public:
		InvalidObjectIdException(std::string func, uint32_t line, uint32_t objId) 
			: Exception(func, line, "id: " + std::to_string(objId)) {
		}
		~InvalidObjectIdException() = default;

		const char* what() const override {
			return "IllegalObjectIdException";
		}
	};

	class InvalidArgumentException : public Exception {
	public:
		InvalidArgumentException(std::string func, uint32_t line, uint32_t objId, std::string reason)
			: Exception(func, line, "id: " + std::to_string(objId) + " " + reason) {
		}
		~InvalidArgumentException() = default;

		const char* what() const override {
			return "InvalidArgumentException";
		}
	};

	class LogicException : public Exception {
	public:
		LogicException(std::string func, uint32_t line, uint32_t objId, std::string reason)
			: Exception(func, line, "id: " + std::to_string(objId) + " " + reason) {
		}
		~LogicException() = default;

		const char* what() const override {
			return "LogicException";
		}
	};
}

