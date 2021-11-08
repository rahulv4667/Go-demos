## Performance Test

To start go server, execute `go run server.go 4242`

To start C++ server, first compile the program with `g++ server.cpp -pthread` and then run it with `./a.out 4343`

Then, you can start benchmarking the programs using either `seq_client.py` or `async_client.py`. The `seq_client.py` sends requests sequentially one after another. `async_client.py` sends requests asynchronously to simulate real life concurrency.

To execute python scripts: `python3 <script_name> <lang_name> <port_num> <requests_num>`
`lang_name` - `cpp` or `go`
`port_num` - `4242` if `go`, `4343` if `cpp`
`requests_num` - number of requests the script needs to send each server.

Benchmarks Obtained:

**Sequential Requests**
| n | Go | C++ |
|---|----|----|
| 50 | 26.97ms | 62ms |
| 100 | 37.6ms | 97.02ms |
| 500 | 242.1ms | 507.86ms |
| 1000 | 503.35ms | 1.49s |
| 5000 | 2.59s | 6.71s |
| 10000 | 6.11s | 14.1s |
| 50000 | 31.16s | connection reset |

\
\
\
**Asynchronous Requests**
| n | Go | C++ |
|---|----|----|
| 50 | 20.52ms | 78.29ms |
| 100 | 51.24ms | 84.53ms |
| 500 | 266.19ms | 460.17ms |
| 1000 | 520.96ms | 1590.19ms |
| 5000 | 3.08s | 7.01s |
| 10000 | 7.69s | 14.55s |
| 50000 | 34.94s | connection reset |