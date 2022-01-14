#!/usr/bin/env python
# coding: utf-8

# In[ ]:


import sys
import argparse
import pandas as pd
import sqlite3
import glob


# In[ ]:


parser = argparse.ArgumentParser("run")
parser.add_argument("test", help="Only the test target has been implemented.", type=str)
parser.add_argument("-d", help="Directory the .db file is in. The most recently created .db file will be tested.", type=str)
args = parser.parse_args()


# In[ ]:


try:
    assert args.test == 'test'
except:
    print("Only the test target has been implemented.\nUse python run.py test -d <dirname>")
    sys.exit(2)


# In[ ]:


files = glob.glob(args.d + '\\*.db')


# In[ ]:


print('Loading database {}...'.format(files[-1]))
cnx = sqlite3.connect(files[-1])


# In[ ]:


il_df = pd.read_sql_query('select * from INPUT_LIBRARIES', cnx)
inputs_df =  pd.read_sql_query('select * from INPUTS', cnx)
ull_df = pd.read_sql_query('select * from COUNTERS_ULL_TIME_DATA', cnx)
string_df =  pd.read_sql_query('select * from COUNTERS_STRING_TIME_DATA', cnx)


# In[ ]:


print('Running input logger tests...')
il_names = il_df['NAME'].values
try:
    assert 'MOUSE' in il_names
except:
    print('mouse_input IL data missing!')
    sys.exit(2)
try:
    assert 'USRWA' in il_names
except:
    print('user_waiting IL data missing!')
    sys.exit(2)
try:
    assert 'FGWND' in il_names
except:
    print('foreground_window IL data missing!')
    sys.exit(2)


# In[ ]:


mouse_x = int(inputs_df[inputs_df['INPUT_NAME'] == 'MOUSE(0)']['ID_INPUT'])
mouse_y = int(inputs_df[inputs_df['INPUT_NAME'] == 'MOUSE(1)']['ID_INPUT'])
mouse_icon = int(inputs_df[inputs_df['INPUT_NAME'] == 'USRWA(0)']['ID_INPUT'])
window = int(inputs_df[inputs_df['INPUT_NAME'] == 'FGWND(0)']['ID_INPUT'])


# In[ ]:


mouse_x_vals = ull_df[ull_df['ID_INPUT'] == mouse_x]['VALUE']
mouse_y_vals = ull_df[ull_df['ID_INPUT'] == mouse_y]['VALUE']
mouse_icon_vals = string_df[string_df['ID_INPUT'] == mouse_icon]['VALUE']
window_vals = string_df[string_df['ID_INPUT'] == window]['VALUE']


# In[ ]:


print('Running mouse_input tests...')
try:
    assert not (mouse_x_vals < 0).any()
except:
    print('mouse_input x values negative!')
    sys.exit(2)
try:
    assert not (mouse_y_vals < 0).any()
except:
    print('mouse_input y values negative!')
    sys.exit(2)


# In[ ]:


print('Running user_waiting tests...')
try:
    assert mouse_icon_vals.str.startswith('IDC').all()
except:
    print('user_waiting has invalid icon values!')
    sys.exit(2)


# In[ ]:


print('Running foreground_window tests...')
try:
    assert window_vals.str.endswith('exe').all()
except:
    print('foreground_window has invalid window values!')
    sys.exit(2)


# In[ ]:


print('All tests passed!')

