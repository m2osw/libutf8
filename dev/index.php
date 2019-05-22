<?php
# Display this directory to users
# (code used to display test results)
#
# Copyright:    Copyright (c) 2011-2019  Made to Order Software Corp.  All Rights Reserved
#
# https://snapwebsites.org/project/advgetopt
# contact@m2osw.com
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#

$dir = glob("advgetopt*");

echo "<html><head><title>advgetopt coverage</title></head>";
echo "<body><h1>advgetopt coverage</h1><table border=\"1\" cellpadding=\"10\" cellspacing=\"0\"><tbody><tr><th>Coverage</th></tr>";
foreach($dir as $d)
{
    echo "<tr>";

    echo "<td><a href=\"", $d, "\">", $d, "</a></td>";

    echo "</tr>";
}
echo "</tbody></table></body></html>";

# vim: ts=4 sw=4 et
