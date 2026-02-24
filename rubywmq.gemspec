require_relative "lib/wmq/version"

Gem::Specification.new do |s|
  s.name        = "rubywmq"
  s.version     = WMQ::VERSION
  s.platform    = Gem::Platform::RUBY
  s.authors     = ["Reid Morrison", "Ajaya Agrawalla"]
  s.email       = ["reidmo@gmail.com"]
  s.homepage    = "https://github.com/reidmorrison/rubywmq"
  s.summary     = "Native Ruby interface into WebSphere MQ"
  s.description = "RubyWMQ is a high performance native Ruby interface into WebSphere MQ."
  s.license     = "Apache-2.0"

  s.metadata = {
    "source_code_uri" => "https://github.com/reidmorrison/rubywmq",
    "bug_tracker_uri" => "https://github.com/reidmorrison/rubywmq/issues"
  }

  s.files = Dir["lib/**/*", "ext/**/*", "LICENSE.txt", "README.md"] -
            Dir["ext/**/*.o", "ext/**/*.so", "ext/Makefile", "ext/wmq_structs.c", "ext/wmq_reason.c"] -
            ["lib/wmq/constants.rb", "lib/wmq/constants_admin.rb"]
  s.extensions    = ["ext/extconf.rb"]
  s.require_paths = ["lib"]

  s.required_ruby_version = ">= 2.5"

  s.requirements << "WebSphere MQ v5.3, v6 or v7 Client or Server with Development Kit"
end
