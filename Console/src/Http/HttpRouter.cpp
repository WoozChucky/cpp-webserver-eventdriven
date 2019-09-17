//
// Created by Nuno Levezinho Silva on 17/09/2019.
//

#include <Console/Http/HttpRouter.hpp>
#include <utility>

void HttpRouter::AddRoute(const std::string& path, const HttpHandler& handler) {

    if (this->_routes.count(path) < 1) {
        Route route;
        route[HttpMethod::GET] = handler;
        route[HttpMethod::POST] = handler;
        route[HttpMethod::DELETE] = handler;
        route[HttpMethod::PUT] = handler;
        this->_routes[path] = route;
    }
}

void HttpRouter::AddRoute(const std::string& path, HttpMethod method, HttpHandler handler) {

    //  TODO(Levezinho)
    //  Check if path exists, if it does check if httpmethod already exists, it it doesnt, add
    //  the handler to that path with the specific method pass by arg. If that method already exists
    // as well, then deny the new route.

    Route route;
    route[method] = std::move(handler);
    this->_routes[path] = route;
}

HttpHandler HttpRouter::GetHandler(const std::string& path, HttpMethod method) {

    auto handler = this->_routes[path][method];

    return handler;
}

HttpHandler HttpRouter::GetHandler(const std::string& path) {

    // defaults to GET method
    auto handler = this->_routes[path][HttpMethod::GET];

    return handler;
}

