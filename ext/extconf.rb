require 'mkmf'
generate_sources_path = File.join(File.dirname(__FILE__), 'generate')
$LOAD_PATH.unshift generate_sources_path
require 'generate_reason'
require 'generate_const'
require 'generate_structs'

include_path, lib_path = dir_config('mqm', '/opt/mqm/inc', '/opt/mqm/lib64')

if have_header('cmqc.h')
  # Generate Source Files
  GenerateReason.generate(include_path+'/')
  GenerateConst.generate(include_path+'/', File.dirname(__FILE__) + '/../lib/wmq')
  GenerateStructs.new(include_path+'/', generate_sources_path).generate
end

$defs << "-DSOEXT=\\\".#{RbConfig::CONFIG["SOEXT"]}\\\""

# Inspired by: https://github.com/brianmario/mysql2/blob/e9c662912dc3bd3707e6c7f0c75e591294cffe12/ext/mysql2/extconf.rb#L263
rpath_flags = " -Wl,-rpath,#{lib_path}"
if RbConfig::CONFIG["RPATHFLAG"].to_s.empty? && try_link('int main() {return 0;}', rpath_flags)
  # Usually Ruby sets RPATHFLAG the right way for each system, but not on OS X.
  $LDFLAGS << rpath_flags
end

# Suppress errors from strict clang warnings on ARM64 macOS
$CFLAGS << " -Wno-incompatible-function-pointer-types -Wno-int-conversion"

# Generate Makefile
create_makefile('wmq/wmq')
