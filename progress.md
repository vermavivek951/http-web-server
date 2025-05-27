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



_____________________________________________________



## Clean Architecture & Config File
- [ ] Split code into modules: server, router, http, logger, utils
- [ ] Use a .conf or .json file to define:
    * Port number
    * Root directory for static files
    * Log file path
- [ ] Add a config parser

## RESTful POST + JSON Body Parsing
- [ ] Accept POST requests
- [ ] Parse request body and Content-Length
- [ ] Parse JSON (manually or with nlohmann/json)

## Response Compression (gzip)
- [ ] Use zlib to compress static file responses
- [ ] Check Accept-Encoding: gzip
- [ ] Set Content-Encoding: gzip in response

## Basic TLS Support (HTTPS)
- [ ] Use OpenSSL or wolfSSL to support HTTPS
- [ ] Serve via https://localhost:4433

## Caching Headers & ETag
- [ ] Set Cache-Control, Expires, Last-Modified, ETag headers
- [ ] Respond with 304 Not Modified when applicable

## Rate Limiting + 404 Logging
- [ ] Limit requests per IP (e.g., max 10 requests/sec/IP)
- [ ] Log all 404s and errors with timestamps and IPs
- [ ] Optional: Block IPs temporarily if abuse detected

## Final Touches
- [ ] Add CLI flags (e.g., --port, --verbose)
- [ ] Add unit tests for HTTP parser