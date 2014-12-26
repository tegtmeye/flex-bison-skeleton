#!/bin/sh

\rm -f dsvdoc_test.xml

./libdsv_test --output_format=XML --log_level=all --report_level=no > libdsv_test.xml

exit `test -f libdsv_test.xml`

