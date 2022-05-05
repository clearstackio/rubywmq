require 'mkmf'
generate_sources_path = File.join(File.dirname(__FILE__), 'generate')
$LOAD_PATH.unshift generate_sources_path
require 'generate_reason'
require 'generate_const'
require 'generate_structs'

include_path, lib_path = dir_config('mqm', '/opt/mqm/inc', '/opt/mqm/lib64')

have_library('mqm')

if have_header('cmqc.h')
  # Generate Source Files
  GenerateReason.generate(include_path+'/')
  GenerateConst.generate(include_path+'/', File.dirname(__FILE__) + '/../lib/wmq')
  GenerateStructs.new(include_path+'/', generate_sources_path).generate
end

# Generate Makefile
create_makefile('wmq/wmq')
