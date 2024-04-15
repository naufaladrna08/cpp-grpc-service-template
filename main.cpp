#include <iostream>
#include <memory>
#include <string>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/strings/str_format.h"

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>

#include <auth.hpp>
#include <authInterceptor.hpp>

ABSL_FLAG(uint16_t, port, 50051, "Server port for the service");

void RunServer(uint16_t port) {
  mongocxx::instance instance{};
  mongocxx::uri uri("mongodb+srv://naufaladrna08:MongoDBSucks31@cluster0.jzgagd5.mongodb.net/?retryWrites=true&w=majority&appName=Cluster0");
  mongocxx::client client(uri);

  std::string server_address = absl::StrFormat("0.0.0.0:%d", port);
  AuthImpl service(&client);

  grpc::EnableDefaultHealthCheckService(true);
  grpc::reflection::InitProtoReflectionServerBuilderPlugin();
  grpc::ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);

  std::vector<std::unique_ptr<grpc::experimental::ServerInterceptorFactoryInterface>> creators;
  creators.push_back(std::unique_ptr<grpc::experimental::ServerInterceptorFactoryInterface>(new AuthInterceptorFactory()));
  builder.experimental().SetInterceptorCreators(std::move(creators));

  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();
}

int main(int argc, char const *argv[]) {
  RunServer(absl::GetFlag(FLAGS_port));
  return 0;
}
