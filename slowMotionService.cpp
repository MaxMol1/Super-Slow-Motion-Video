#include "slowMotionService.hpp"

#include <zhelpers.hpp>
#include <iostream>

using namespace std;

SlowMotionService::SlowMotionService(string input_path, int slowmo_factor, int output_fps, string output_path) {
    // create context
    this->context = make_unique<zmq::context_t>(1);

    // initialize requester socket on localhost:8080
    this->flow_requester = make_unique<zmq::socket_t>(*context, ZMQ_REQ);
    cout << "SMS: Connecting requester to tcp://127.0.0.1:5555..." << endl;
	flow_requester->connect("tcp://127.0.0.1:5555");

    // initialize variables
    this->input_path = input_path;
    this->slowmo_factor = slowmo_factor;
    this->output_fps = output_fps;
    this->output_path = output_path;

    // TODO: mkdir tmps

    // TODO: create frame reader form input_path and slowmo_factor

    // TODO: create models
}

// TODO: rewrite this method
void SlowMotionService::startService() {
    int first_frame_index = 0;
    // TODO
    // lastFrameIndex = frr.getFrameCount() * args.slowdown
    int last_frame_index = 20;

    while (first_frame_index < last_frame_index) {
        // send request with frame number
        s_send(*flow_requester, to_string(first_frame_index));
        
        // receive flow vector message
        string msg = s_recv(*flow_requester);

        cout << "received " << msg[0] << msg[1] << endl;

        // TODO: decode msg

        // TODO: process msg

        // TODO: make intermediate frames

        first_frame_index++;
    }

    // send termination request
    s_send(*flow_requester, to_string(-1));

    // TODO: reconstruct video

    return;
}
