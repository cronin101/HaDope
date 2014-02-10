#!/usr/bin/env ruby
require 'mkmf'

$objs = %w{lib/oclerrorexplain.o lib/prefix_sum/prescan.o lib/hadope.o hadope_backend.o}

extension_name = 'hadope_backend'

dir_config extension_name

have_header 'lib/hadope.h'
have_header 'lib/oclerrorexplain.h'
have_header 'lib/prefix_sum/prescan.h'

case Gem::Platform.local.os
when 'darwin'
  $LIBS << ' -framework OpenCL'
else
  have_library 'OpenCL'
end

create_header
create_makefile extension_name
