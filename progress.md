# C++ HTTP Server Project Progress

## TCP Socket Server
- [X] Bind and listen
- [X] Accept client
- [X] Read raw HTTP request
- [X] Send basic HTTP response

## HTTP Parsing
- [X] Parse method and path
- [X] Extract headers
- [X] Handle malformed requests

## Static File Serving
- [X] Read files
- [X] MIME type detection
- [X] Return 404 if missing

## Routing
- [X] Map paths to handlers
- [X] Return JSON for `/api/...`

## Multithreading
- [X] Handle concurrent clients
- [X] Clean up threads

## Errors & Logging
- [X] Proper status codes
- [X] Add logging

## Final Polish
- [ ] Handle query params
- [ ] Support POST  (optional)
- [ ] Refactor and document
