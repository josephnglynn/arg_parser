project(
        'arg_parser',
        'cpp',
        version: '0.0.1', 
        default_options : [
            'warning_level=3',
            'cpp_std=c++17'
        ]
)

includes = include_directories('.')
src = ['arg_parser/parser.cpp']
deps = []
   
arg = ''
if get_option('buildtype').startswith('debug')
   arg = '-DDEBUG'
endif


add_project_arguments(arg, language: 'cpp')

arg_parser = shared_library(
    'arg_parser',
    src,
    include_directories : includes,
    dependencies: deps,
)


arg_parser_dep = declare_dependency(include_directories: includes, link_with: arg_parser)

subdir('tests')