#!/bin/bash
diff <(make test) <(cat tester/tests/res/joins_the_server)
