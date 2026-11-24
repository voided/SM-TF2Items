import argparse
import re
import sys

try:
    from ambuild2 import run
except ImportError:
    sys.exit('Install AMBuild 2.2 or newer from https://github.com/alliedmodders/ambuild.')

if getattr(run, 'CURRENT_API', '2.1').startswith('2.1'):
    sys.exit('AMBuild 2.2 or newer is required.')

def release_version(value):
    if not re.fullmatch(r'[0-9]+\.[0-9]+\.[0-9]+(?:\.[0-9]+)?', value):
        raise argparse.ArgumentTypeError('expected MAJOR.MINOR.PATCH[.BUILD]')
    return value

parser = run.BuildParser(sourcePath=sys.path[0], api='2.2')
parser.options.add_argument('--sm-path', required=True, help='Path to SourceMod')
parser.options.add_argument('--mms-path', required=True, help='Path to Metamod:Source')
parser.options.add_argument('--hl2sdk-path', required=True, help='Path to the TF2 HL2SDK')
parser.options.add_argument('--manager-plugin', required=True,
                            help='Path to the compiled tf2items_manager.smx')
parser.options.add_argument('--version', type=release_version,
                            help='Version baked into the extension (defaults to product.version)')
parser.options.add_argument('--enable-debug', action='store_true', help='Enable debug builds')
parser.options.add_argument('--enable-optimize', action='store_true', help='Enable optimization')
parser.Configure()
