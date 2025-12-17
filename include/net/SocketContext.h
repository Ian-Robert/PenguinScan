#pragma once

class SocketContext {
public:
	SocketContext();
	~SocketContext();

	// Delete Copy Constructor and Assignment Operator for Safety. Makes sure the objects cant be copied
	SocketContext(const SocketContext&) = delete;
	SocketContext& operator=(const SocketContext&) = delete;
};