[Building Your First Service with Apache Thrift: A Practical Tutorial](https://medium.com/@tahir.rauf/building-your-first-service-with-apache-thrift-a-practical-tutorial-1e591c4af8d9)

[Apache Thrift Tutorial](https://www.tutorialspoint.com/apache-thrift/index.htm)

Apache Thrift is a software framework designed to facilitate cross-language service development. It provides what is essentially a remote-procedure call interface to enable a client application to access services from a service — which can be written in the-same, or another language.

It provides a simple interface definition language (IDL) to define data types and service interfaces in a language-neutral way which are stored as .thrift files. Using these definitions, Thrift’s code generation engine can automatically generate client and server code for a wide variety of programming languages, making it easier to build distributed systems whose components communicate seamlessly, regardless of the underlying language they are written in.

## IDL (Interface Definition Language)
IDL is used to define communication interfaces in a language-neutral manner. Supported types include:

* Base Types
* bool: Boolean value
* byte: 8-bit signed integer
* i16, i32, i64: 16, 32, and 64-bit signed integers
* double: 64-bit floating-point number
* string: UTF-8 encoded text string

### Structs
A struct has a set of strongly typed fields, each with a unique name as an identifier.

### Containers
* list: Ordered list of elements
* set: Unordered set of unique elements
* map<type1, type2>: Map of unique keys to values

### Exception
Exceptions are functionally equivalent to structs, except that they inherit from the native exceptions.

### Services
Services are actually communication interfaces defined using Thrift types. They consist of a set of named functions, each with a list of parameters and a return type.

## Service Description file
A Thrift service named Logger could be defined as follows:
```
namespace py LoggerPy
namespace cpp LoggerCpp

exception LoggerException
{
    1: i32 error_code,
    2: string error_description
}

service Logger
{
    oneway void timestamp (1: string filename)
    string get_last_log_entry (1: string filename) throws (1: LoggerException error)
    void write_log (1: string filename, 2: string message) throws (1: LoggerException error)
    i32 get_log_size (1: string filename) throws (1: LoggerException error)
}
```

Namespaces

* namespace cpp logger instructs the Thrift compiler that when generating C++ code, it should place the generated code within a C++ namespace named LoggerCpp .
* namespace py LoggerPyinstructs the compiler that when generating Python code, it should create a Python package or module structure under LoggerPy.

## Custom Exception:
exception LoggerException defines a custom exception LoggerException that the server can throw to indicate errors. It includes error_code (A machine-readable code representing the type of error) and error_description (A human-readable message describing the error.
This exception can be returned by the service methods that may fail due to invalid input, internal errors, or file-related issues.

Note: Each field within a struct or exception is assigned a unique numeric ID. These IDs serve a couple of important purposes

* Identification of Fields in the Binary Protocol: When Thrift serializes data, it uses these field IDs. Having numeric IDs makes it easier for Thrift to know which bytes correspond to which field, regardless of the field’s name.

* Field IDs allow you to evolve your data structures over time. If you later add, remove, or reorder fields in your Thrift definition, the field IDs still ensure that old clients and servers can understand the data. For example, if you add a new field with a new unique ID in a future version, old clients just ignore that field since they don’t recognize the ID.

## Service definition
service Logger {...} defines a Thrift service named Logger. A service is essentially a collection of remote procedures that client can call on the servers.

### Methods
oneway: Keyword means that the client will send this request to the server but will not wait for any response or acknowledgement. It’s a ‘fire and forget’ call.

In summary, this Thrift service Logger likely represents a simple logging system. It allows clients to send log messages to a server, retrieve log entries, get log sizes.

## Code Generation and Usage
To generate client and server code, run:
```
thrift --gen cpp LoggerService.thrift
```

When you run the Thrift compiler with the -gen cpp option, it generates


* xxx_types.h and xxx_types.cpp
These are DataType classes.
For each thrift-defined struct, exception, the compiler generates corresponding C++ classes. Along with member variables and methods, it includes serialization, deserialization, equality comparison and printing methods.
In our case, it generates LoggerService_types.h and .cpp files.

* xxx.h and xxx.cpp
Interface classes: A pure virtual interface class LoggerIf that defines the methods your server-side implementation must provide.
Processor A class LoggerProcessor that reads incoming requests, dispatches them to your handler (an implementation of the If interface) and writes out responses.
Client A client stub class LoggerClient that provides typed methods for calling the remote service. This class handles the serialization of the arguments, sending requests and parsing returned response.
Serialization/Deseralization Each generated class includes methods to
write: Serialize the object’s data into Thrift’s binary or compact protocol format.
read: Deserialize the object’s data from the binary stream back into c++ class fields.
Constants If your Thrift file defines const values, a seperate C++ file and header will be generated (e.g constants.h and constants.cpp)
In our case, it generates Logger.h and Logger.cpp files.

* Logger_server.skeleton.cpp
A dummy version of the service definition.

## Code generation summary
For a file named LoggerService.thrift, you will end up with files like:

* LoggerService_constants.h / LoggerService_constants.cpp
* LoggerService_types.h / LoggerService_types.cpp (for structs, enums, exceptions)
* LoggerService.h / LoggerService.cpp (for the service interface, processor, and client)

These headers and source files can then be included into your client and server projects.

## Server and Client implementation
### Server Implementation
Thrift compiler has generated the Logger_server.skeleton.cpp file. Recommended practice is to make a copy of the ___.skeleton.cpp and to build out from there. We’ll create a copy of server skeleton file and name it LoggerServer.cpp .
LoggerHandler class will implement the If interface
```
class LoggerHandler : public LoggerIf {
public:
    void timestamp(const std::string& filename) override {
        // Implementation
    }
    // Other methods...
};
```

LoggerProcessor A TProcessor is a class generated by Thrift to handle incoming RPC calls. The LoggerProcessor takes the incoming requests, decodes them, and routes them to the appropriate method in the LoggerHandler.
Server Transport Components TServerSocket is a transport layer that listens for incoming connections on a specific port (9090 in this case).
TSimpleServer is a simple, single-threaded server provided by Thrift. It listens for client connections, processes requests synchronously, and sends back responses.

```
#include "gen-cpp/Logger.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;
using namespace ::LoggerCpp;
class LoggerHandler : virtual public LoggerIf {
public:
LoggerHandler() {
}
void timestamp(const std::string& filename) {
printf("timestamp\n");
}
void get_last_log_entry(std::string& _return, const std::string& filename) {
printf("get_last_log_entry\n");
printf("filename = %s\n", filename.c_str());
}
void write_log(const std::string& filename, const std::string& message) {
printf("write_log\n");
printf("filename = %s, message = %s\n", filename.c_str(), message.c_str());
}
int32_t get_log_size(const std::string& filename) {
printf("get_log_size\n");
return 0;
}
};

int main(int argc, char **argv) {
int port = 9090;
::std::shared_ptr<LoggerHandler> handler(new LoggerHandler());
::std::shared_ptr<TProcessor> processor(new LoggerProcessor(handler));
::std::shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
::std::shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
::std::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());
TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
server.serve();
return 0;
}
```
### Client Implementation
The client relies on the generated LoggerClient class to call the service methods.
* TSocket creates a socket connection to the server on localhost (127.0.0.1) and port 9090. This is the communication endpoint for the client.

* TBinaryProtocol specifies the serialization protocol used for encoding/decoding messages.

* LoggerClient class is generated from the Thrift IDL and contains methods corresponding to the Logger service (timestamp, write_log, etc.). The client uses the protocol to send requests and receive responses.

```
#include <iostream>
#include "gen-cpp/Logger.h" 
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/protocol/TBinaryProtocol.h>
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace LoggerCpp;

int main(int argc, char **argv)
{
    char logfile[]="logfile.log";
    std::string line;

    std::shared_ptr<TSocket> socket(new TSocket("localhost", 9090));
    std::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
    std::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

    LoggerClient client(protocol);
    try {
        transport->open();

        client.timestamp(logfile);
        std::cout << "Logged timestamp to log file" << std::endl;

        client.write_log(logfile, "This is a message that I am writing to the log");
        client.timestamp(logfile);

        client.get_last_log_entry(line, logfile);
        std::cout << "Last line of the log file is: " << line << std::endl;
        std::cout << "Size of log file is: " << client.get_log_size(logfile) << " bytes" << std::endl;

        transport->close();
    }
    catch (TTransportException e) {
        std::cout << "Error starting client" << std::endl;
    }
    catch (LoggerException e) {
        std::cout << e.error_description << std::endl;
    }
    return 0;
}
```

### Building and Running the Application
Create the CMakeLists.txt file
```
cmake_minimum_required(VERSION 3.5)
project(LoggerService)

# Use C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Additional compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -DHAVE_INTTYPES_H -DHAVE_NETINET_IN_H -Wno-deprecated-declarations")

# Update paths for Apple Silicon Homebrew prefix
include_directories(${CMAKE_SOURCE_DIR})
include_directories("/opt/homebrew/include")

link_directories("/opt/homebrew/lib")

set(BASE_SOURCE_FILES 
    gen-cpp/Logger.cpp 
    gen-cpp/LoggerService_types.cpp 
)

set(SERVER_FILES LoggerServer.cpp)
set(CLIENT_FILES LoggerClient.cpp)

add_executable(LoggerServer ${SERVER_FILES} ${BASE_SOURCE_FILES})
add_executable(LoggerClient ${CLIENT_FILES} ${BASE_SOURCE_FILES})
target_link_libraries(LoggerServer thrift)
target_link_libraries(LoggerClient thrift)
```
To build, go to gen-cpp directory and issue cmake .. and make .


### Run the Server and Client

Start the server: ./LoggerServer
Execute the client: ./LoggerClient


------------------------------------------------------------------------------------

## Another example 

Perfect! Let’s build a **real-world example** where:

- 🖥️ **Server is in C++**
- 🐍 **Client is in Python**
- 📦 Communication is done using **Apache Thrift**

We’ll walk through everything:
- File structure
- `.thrift` definition
- C++ server
- Python client
- Build + run

---

## 🗂️ Project Structure

```
thrift-cpp-server-python-client/
├── user_service.thrift
├── server/
│   └── main.cpp
├── client/
│   └── client.py
├── gen-cpp/        <-- Generated by Thrift
├── gen-py/         <-- Generated by Thrift
└── CMakeLists.txt
```

---

## ✏️ Step 1: Thrift IDL File

**File**: `user_service.thrift`

```thrift
namespace cpp user_service
namespace py user_service

struct User {
  1: i32 id,
  2: string name
}

service UserService {
  User getUser(1: i32 id)
}
```

---

## ⚙️ Step 2: Generate Code

Generate both C++ and Python code:

```bash
thrift --gen cpp user_service.thrift
thrift --gen py user_service.thrift
```

This will generate:
- `gen-cpp/`: C++ files
- `gen-py/`: Python files

---

## 🖥️ Step 3: C++ Server Code

**File**: `server/main.cpp`

```cpp
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>

#include "../gen-cpp/UserService.h"

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;

using namespace user_service;

class UserServiceHandler : public UserServiceIf {
public:
    void getUser(User& _return, const int32_t id) override {
        _return.id = id;
        _return.name = "C++ Alice #" + std::to_string(id);
    }
};

int main() {
    auto handler = std::make_shared<UserServiceHandler>();
    auto processor = std::make_shared<UserServiceProcessor>(handler);
    auto serverTransport = std::make_shared<TServerSocket>(9090);
    auto transportFactory = std::make_shared<TBufferedTransportFactory>();
    auto protocolFactory = std::make_shared<TBinaryProtocolFactory>();

    TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
    std::cout << "C++ server running on port 9090..." << std::endl;
    server.serve();
    return 0;
}
```

---

## 🧱 Step 4: CMake File for Server

**File**: `CMakeLists.txt`

```cmake
cmake_minimum_required(VERSION 3.10)
project(ThriftCppServer)

set(CMAKE_CXX_STANDARD 17)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/gen-cpp
    /usr/local/include/thrift
)

link_directories(/usr/local/lib)

add_executable(server server/main.cpp gen-cpp/UserService.cpp gen-cpp/user_service_types.cpp)
target_link_libraries(server thrift)
```

Build:
```bash
mkdir build
cd build
cmake ..
make
```

Run server:
```bash
./server
```

---

## 🐍 Step 5: Python Client

Install thrift:
```bash
pip install thrift
```

**File**: `client/client.py`

```python
from thrift.transport import TSocket
from thrift.transport import TTransport
from thrift.protocol import TBinaryProtocol

from gen_py.user_service import UserService
from gen_py.user_service.ttypes import User

def main():
    # Connect to C++ server
    transport = TSocket.TSocket('localhost', 9090)
    transport = TTransport.TBufferedTransport(transport)
    protocol = TBinaryProtocol.TBinaryProtocol(transport)
    client = UserService.Client(protocol)

    transport.open()
    user = client.getUser(1)
    print(f"[Python] Got user from C++ server: {user.name}")
    transport.close()

if __name__ == '__main__':
    main()
```

---

## ✅ Final Steps

1. Start your **C++ server**:
   ```bash
   ./build/server
   ```

2. In another terminal, run **Python client**:
   ```bash
   python3 client/client.py
   ```

You should see:
```
[Python] Got user from C++ server: C++ Alice #1
```

---

## 🧠 Recap

| Part | Tech |
|------|------|
| Definition | `user_service.thrift` |
| Server | C++ with `TSimpleServer` |
| Client | Python with `UserService.Client()` |
| Communication | Fast binary via Thrift protocol |

---

Would you like me to zip and send a downloadable project starter for this setup? Or show how to reverse it (Python server and C++ client)?