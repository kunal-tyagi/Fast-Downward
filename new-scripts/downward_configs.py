"""
Example configurations taken from 
http://alfons.informatik.uni-freiburg.de/downward/PlannerUsage
"""
import sys
import logging
from collections import defaultdict

import experiments
import tools
import downward_suites

HELP = """\
Configurations can be specified in the following ways: \
ou, astar_searches, myconfigfile:yY, myconfigfile:lama_configs
The python modules have to live in the scripts dir.
"""

# Eager A* search with landmark-cut heuristic (previously configuration ou)
ou = '--search "astar(lmcut())"'

fF = """\
--heuristic "hff=ff()" \
--search "lazy_greedy(hff, preferred=(hff))"\
"""

yY = """\
--heuristic "hcea=cea()" \
--search "lazy_greedy(hcea, preferred=(hcea))"\
"""

fFyY = """\
--heuristic "hff=ff()" --heuristic "hcea=cea()" \
--search "lazy_greedy(hff, hcea, preferred=(hff, hcea))"\
"""

lama = """\
--heuristic "hff=ff()" --heuristic "hlm=lmcount()" --search \
"iterated(lazy_wastar(hff,hlm,preferred=(hff,hlm),w=10),\
lazy_wastar(hff,hlm,preferred=(hff,hlm),w=5),\
lazy_wastar(hff,hlm,preferred=(hff,hlm),w=3),\
lazy_wastar(hff,hlm,preferred=(hff,hlm),w=2),\
lazy_wastar(hff,hlm,preferred=(hff,hlm),w=1),\
repeat_last=true)"\
"""

blind = """\
--search "astar(blind())"\
"""

oa50000 = """\
--search "astar(mas())"\
"""

def astar_searches():
    return [('blind', blind), ('oa50000', oa50000)]
    
    



def get_configs(configs_strings):
    """
    Parses configs_strings and returns a list of tuples of the form 
    (configuration_name, configuration_string)
    
    config_strings can contain strings of the form 
    "configs.py:cfg13" or "configs.py"
    """        
    all_configs = []
        
    files_to_configs = defaultdict(list)
    for config_string in configs_strings:
        if ':' in config_string:
            config_file, config_name = config_string.split(':')
        else:
            # Check if this module has the config
            config_file, config_name = __file__, config_string
            
        files_to_configs[config_file].append(config_name)
    
    for file, config_names in files_to_configs.iteritems():
        module = tools.import_python_file(file)
        module_dict = module.__dict__
        for config_name in config_names:
            config_or_func = module_dict.get(config_name, None)
            if config_or_func is None:
                msg = 'Config "%s" could not be found in "%s"' % (config_name, file)
                logging.error(msg)
                sys.exit()
            try:
                config_list = config_or_func()
            except TypeError:
                config_list = [(config_name, config_or_func)]
            
            all_configs.extend(config_list)
    
    logging.info('Found configs: %s' % all_configs)
    return all_configs
    
    
def get_dw_parser():
    '''
    Returns a parser for fast-downward experiments
    '''
    # We can add our own commandline parameters
    dw_parser = experiments.ExpArgParser()
    dw_parser.add_argument('-s', '--suite', default=[], nargs='+', 
                            required=True, help=downward_suites.HELP)
    dw_parser.add_argument('-c', '--configs', default=[], nargs='+', 
                            required=True, help=HELP)
    return dw_parser
    
    
if __name__ == '__main__':
    get_configs(['blind', 'downward_configs:astar_searches'])

