#include <auth.hpp>

AuthImpl::AuthImpl(mongocxx::client* client) {
  if (client == nullptr) {
    throw std::runtime_error("Client is null");
  }

  m_client = client;
  if (m_client != nullptr) {
    std::cout << "MongoDB instance created" << std::endl;
  }
}

grpc::Status AuthImpl::HelloWorld(grpc::ServerContext* context, const HelloWorldRequest* request, HelloWorldResponse* response) {
  std::string prefix("Hello ");
  response->set_message(prefix + request->additional());

  try {
    auto collection = m_client->database("test").collection("users");
    auto User = collection.find_one({});
    if (User) {
      response->add_users(User->view()["username"].get_string().value.to_string());
    } 
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return grpc::Status(grpc::StatusCode::INTERNAL, e.what());
  }

  return grpc::Status::OK;
}

grpc::Status AuthImpl::Login(grpc::ServerContext* context, const LoginRequest* request, LoginResponse* response) {
  try {
    auto collection = m_client->database("test").collection("users");
    auto User = collection.find_one(bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("username", request->username())));

    if (User) {
      if (bcrypt::validatePassword(request->password().c_str(), User->view()["password"].get_string().value.to_string().c_str()) == 0) {
        return grpc::Status(grpc::StatusCode::UNAUTHENTICATED, "Invalid password");
      }

      std::string token = jwt::create()
        .set_issuer("auth0")
        .set_type("JWS")
        .set_payload_claim("username", jwt::claim(request->username()))
        .sign(jwt::algorithm::hs256{"secret"});

      response->set_message("Login success");
      response->set_token(token);
    } else {
      return grpc::Status(grpc::StatusCode::NOT_FOUND, "User not found");
    }
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return grpc::Status(grpc::StatusCode::INTERNAL, e.what());
  }

  return grpc::Status::OK;
}

grpc::Status AuthImpl::Register(grpc::ServerContext* context, const RegisterRequest* request, RegisterResponse* response) {
  try {
    auto collection = m_client->database("test").collection("users");
    auto User = collection.find_one(bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("username", request->username())));

    if (User) {
      return grpc::Status(grpc::StatusCode::ALREADY_EXISTS, "User already exists");
    }

    std::string hashedPassword = bcrypt::generateHash(request->password().c_str());
    collection.insert_one(bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("username", request->username()), bsoncxx::builder::basic::kvp("password", hashedPassword)));

    std::string token = jwt::create()
      .set_issuer("auth0")
      .set_type("JWS")
      .set_payload_claim("username", jwt::claim(request->username()))
      .sign(jwt::algorithm::hs256{"secret"});

    response->set_message("User created");
    response->set_token(token);
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return grpc::Status(grpc::StatusCode::INTERNAL, e.what());
  }

  return grpc::Status::OK;
}