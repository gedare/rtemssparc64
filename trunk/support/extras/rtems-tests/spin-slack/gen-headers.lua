-- gen-headers.lua
--
-- Generate header files for:
--  spslackXX   -- slackparams.h, slackmacros.h
--  spedfXX     -- edfparams.h, edfmacros.h
--  spspinXX    -- params.h, macros.h
--

function usage(arg)
  print(string.format('%q', 'Usage: lua '..arg[0]..' -T d,e,p ... -A d,e,r ...'))
  print(" -T d,e,p a periodic task with deadline d, execution time e, "
    .. "and phase p: \n\t\tNOTE: phase should be set to 0, or slack computation might be off.")
  print(" -A d,e,r an aperiodic task with deadline d, execution time e, and release time r: \n\t\tNOTE: deadline is not supported, but is used to assign priority.")
  print(" -T and -A can be repeated as many times as necessary")
end

function parse_args(arg)
  local t = {}
  local args = table.concat(arg," ")
  for w in string.gmatch(args, "-%a%s[%w+%.?%w*,]+") do  
    table.insert(t, w)
  end
  return t
end

function print_table(t)
  if t then
    for i,v in ipairs(t) do
      if type(v) == "table" then
        io.write(string.format('Sub-table %s\n', tostring(i)))
        print_table(v)
        print('\End sub-table')
      else
        print(string.format('%s', tostring(i)..'\t'..tostring(v)))
      end
    end
  end
end

function map(f, ...)
  local function helper(f, n, a, ...)
    if n > 0 then return f(a), helper(f, n-1, ...) end
  end
  return helper(f, select('#', ...), ...)
end

function task_lt(task1, task2)
  if string.match(task1, "-A") then
    if string.match(task2, "-P") then
      return true
    end
  elseif string.match(task2, "-A") then
    return true
  end

  local p1 = get_task_params(task1)
  local p2 = get_task_params(task2)

  -- sort first by deadline, then by release.
  if p1[1] < p2[1] then
    return true
  elseif p1[1] == p2[1] and p1[3] < p2[3] then
    return true
  end

  return false
end

-- return a table with entries of (a)periodic tasks
-- Precondition: 
--    Tasks is a table of task descriptions.
--    Periodic tasks are represented by an entry of (-A r,e) -T d,e,p
--    where d, e, and p are numeric values representing deadline (period), 
--    execution time, and phase respectively.
-- Postcondition:
--    Returns a new table with only the periodic tasks.
function get_periodic_tasks(tasks)
  local t = {}
  if (tasks) then
    for i,v in ipairs(tasks) do
      if string.match(v, "-T") then
        table.insert(t, v)
      end
    end
  end
  return t
end

function get_aperiodic_tasks(tasks)
  local t = {}
  if (tasks) then
    for i,v in ipairs(tasks) do
      if string.match(v, "-A") then
        table.insert(t, v)
      end
    end
  end
  return t
end

-- extracts a table containing the parameters (d,e,p) or (r,e) of task.
function get_task_params(task)
  local t = {}
  local i = 1
  if (task) then
    local s = tostring(task)
    for w in string.gmatch(s, "%d+%.?%d*") do
      t[i] = tonumber(w)
      i = i + 1
    end
  end
  return t
end

-- extracts a table of deadlines (periods) from the array of periodic tasks
-- passed as argument
function get_deadlines(tasks)
  local t = {}
  for i, v in ipairs(tasks) do
    local d = get_task_params(v)[1]
    t[i] = d
  end
  return t
end

function get_release_times(tasks)
  local t = {}
  for i, v in ipairs(tasks) do
    local d = get_task_params(v)[3]
    t[i] = d
  end
  return t
end

-- extracts tables of execution times from array of tasks passed as arg.
function get_execution_times(tasks)
  local t = {}
  for i, v in ipairs(tasks) do
    local d = get_task_params(v)[2]
    t[i] = d
  end
  return t
end 

-- finds the greatest common divisor of two numbers. could be more efficient.
function gcd_2(a, b)
  if (a < b) then
    a,b = b,a
  end
  if (b == 0) then
    return a
  end
  return (gcd_2(b, a - b * math.floor(a/b)))
end

function lcm_2(a, b)
  return a*b / gcd_2(a,b)
end

-- finds the least common multiple among a set of numbers
function lcm(...)
  local last_v = 1
  local last_m = 1

  for _, v in ipairs({...}) do
    local m = lcm_2(last_v, v)
    last_m = lcm_2(m, last_m)
    last_v = v
  end
  return last_m
end

-- finds the hyperperiod length
-- Precondition:
--    task_periods is a table of periodic task period durations
-- Postcondition:
--    Returns the length of the hyperperiod.
function get_hyperperiod_length(task_periods)
  return lcm(map(tonumber,unpack(task_periods)))
end

-- add job to J based on earliest deadline first. 
-- Use smaller release time to break ties.
function insert_sort_jobs(J, job)
  -- J is a table of jobs. Each job is a table like:
  -- 1  deadline time
  -- 2  execution time
  -- 3  release time
  for i,v in ipairs(J) do
    if job[1] < v[1] then
      table.insert(J,i,job)
      return
    elseif job[1] == v[1] then
      if job[3] < v[3] then
        table.insert(J,i,job)
        return
      end
    end
  end

  -- empty table or reached end of table
  table.insert(J,job)
  return
end

-- returns a table where entry t[i] is the i'th job of a hyperperiod
-- Precondition:
--    ptasks is a table of periodic task decriptions
--    hpl is the duration of a hyperperiod
-- Postcondition:
--    Returns a table with job descriptions, sorted by deadline.
function get_jobs(ptasks, hpl)
  local J = {}
  
  -- iterate over periodic tasks. For each task, add the jobs in a hyperperiod
  -- to the table J as: deadline, execution, release
  for _,p in ipairs(ptasks) do
    local task = get_task_params(p)
    local period = tonumber(task[1])
    local e = tonumber(task[2])
    -- TODO: phase is ignored ...

    local d = period
    local r = 0
    -- compute jobs
    while (d <= hpl) do
      local job = {}
      local mt = {}
      table.insert(job, d)
      table.insert(job, e)
      table.insert(job, r)
      insert_sort_jobs(J, job)
      d = d + period
      r = r + period
    end
  end

  return J
end


function compute_initial_slacks(J)
  for i,job in ipairs(J) do
    local d = job[1]
    local e = 0
    local sigma = 0
    for _, j in ipairs(J) do
      if (j[1] <= job[1]) then
        e = e + j[2]
      end
    end
    sigma = d - e
    assert(sigma >= 0,"negative slack computed\n")
    table.insert(job,sigma)
  end
end

-- computes the slack table given the table J of jobs ordered by deadline,
-- where each entry of J is a table representation of a job with elements J[i]:
--  J[i][1] = deadline
--  J[i][2] = execution
--  J[i][3] = release time
--  J[i][4] = sigma_i(0)
-- 
-- This function returns a flat array of the precomputed slacks.
function compute_slack_table(J)
  local t = {}
  
  for i,job in ipairs(J) do
    for j = i, #J, 1 do
      -- omega(i,j) = min(sigma_k(0)) for k = i ... j
      local omega = job[4]
      for k = i, j, 1 do
        if J[k][4] < omega then
          omega = J[k][4]
        end
      end
      table.insert(t,omega)
    end
  end
  return t
end

function write_header(atasks, ptasks, J, slack_table, hpl)
  local f,h
  if (slack_table) then
    f = io.open("slackparams.h","w")
    h = io.open("slackmacros.h","w")
  else
    f = io.open("params.h","w")
    h = io.open("macros.h","w")
  end

  -- print comment header block
  h:write("/* macros.h\n *\n * This is a generated file.\n * Use the gen-headers.lua script to create this file.\n */\n\n")

  h:write("/* This file was generated with the following parameters: \n")
  h:write(table.concat(ptasks, " "))
  h:write(" ")
  h:write(table.concat(atasks, " "))
  h:write("\n*/\n\n")

  h:write("#ifndef __MACROS_H_\n#define __MACROS_H_\n\n")

  -- pre-computed size macros
  if (slack_table) then
    h:write(string.format("#define  TABLE_SIZE                 (%d)\n", 
    #slack_table))
  end
  h:write(string.format("#define  JOBS_PER_HP                (%d)\n", 
  #J))
  h:write(string.format("#define  HP_LENGTH                  (%d)\n", 
  hpl))
  h:write(string.format("#define  NUM_PERIODIC_TASKS         (%d)\n", 
  #ptasks))
  h:write(string.format("#define  NUM_APERIODIC_TASKS        (%d)\n",
  #atasks))
  h:write(string.format("#define  NUM_TASKS                  ( NUM_PERIODIC_TASKS + NUM_APERIODIC_TASKS )\n"))

  h:write(string.format("#define  THRESHOLD_US      ( HP_LENGTH * 2 * CONFIGURE_MICROSECONDS_PER_TICK )\n"))

  h:write("#endif\n")

  h:flush()
  h:close()

  -- print comment header block
  f:write("/* params.h\n *\n * This is a generated file.\n * Use the gen-headers.lua script to create this file.\n */\n\n")

  f:write("/* This file was generated with the following parameters: \n")
  f:write(table.concat(ptasks, " "))
  f:write(" ")
  f:write(table.concat(atasks, " "))
  f:write("\n*/\n\n")
  f:write("#ifndef __PARAMS_H_\n#define __PARAMS_H_\n\n")

  -- pre-computed size macros
  if (slack_table) then
    f:write(string.format("#define  TABLE_SIZE                 (%d)\n", 
    #slack_table))
  end
  f:write(string.format("#define  JOBS_PER_HP                (%d)\n", 
  #J))
  f:write(string.format("#define  HP_LENGTH                  (%d)\n", 
  hpl))
  f:write(string.format("#define  NUM_PERIODIC_TASKS         (%d)\n", 
  #ptasks))
  f:write(string.format("#define  NUM_APERIODIC_TASKS        (%d)\n",
  #atasks))
  f:write(string.format("#define  NUM_TASKS                  ( NUM_PERIODIC_TASKS + NUM_APERIODIC_TASKS )\n"))

  f:write(string.format("#define  THRESHOLD_US      ( HP_LENGTH * 2 * CONFIGURE_MICROSECONDS_PER_TICK )\n"))

  -- slack table
  if slack_table then
    f:write(string.format("\n%s", 
    "/* pre-computed slack table */\n"))
    f:write(string.format("uint32_t  slack_ticks[TABLE_SIZE]   = { %s };\n", 
    table.concat(slack_table, ", ")))

    f:write("/* slack_table gets converted from ticks to timestamp later */\n")
    f:write("Timestamp_Control slack_table[TABLE_SIZE];\n")
  end

  -- miscellaneous globals
  f:write("\nuint32_t  jobs_per_hyperperiod      = JOBS_PER_HP;\n")
  f:write("uint32_t  periodic_tasks            = NUM_PERIODIC_TASKS;\n\n")

  local periods = get_deadlines(ptasks);
  local prio = get_deadlines(atasks);

  f:write(string.format("rtems_task_priority Priorities[1+NUM_TASKS]= { 0"))
  if #periods > 0 then
    f:write(string.format(", %s", table.concat(periods, ", ")))
  end
  if #prio > 0 then
    f:write(string.format(", %s", table.concat(prio, ", ")))
  end
  f:write(" };\n\n")

  if #periods > 0 then
    f:write(string.format("uint32_t  Periods[1+NUM_PERIODIC_TASKS]    = { %s };\n",
    "0, " .. table.concat(periods, ", ")))
  else
    f:write(string.format("uint32_t  Periods[1+NUM_PERIODIC_TASKS]    = { 0 };\n"))
  end

  if #periods > 0 then
    f:write(string.format("%s", 
    "uint32_t  Periods_us[1+NUM_PERIODIC_TASKS] = {\n" ..
    "                                             " ..
    "0*CONFIGURE_MICROSECONDS_PER_TICK,\n" ..
    "                                             " ..
    table.concat(periods, "*CONFIGURE_MICROSECONDS_PER_TICK,\n                                             ") ..
    "*CONFIGURE_MICROSECONDS_PER_TICK\n" ..
    "                                             };\n")) 
  else
    f:write("uint32_t  Periods_us[1+NUM_PERIODIC_TASKS] = { 0 };\n");
  end

  f:write("\n")

  local p_execution = get_execution_times(ptasks);
  local a_execution = get_execution_times(atasks);

  if #p_execution > 0 then
    f:write(string.format("%s", 
    "uint32_t  Execution_threshold[1+NUM_PERIODIC_TASKS] = {\n" ..
    "                " .. "0,\n" .. "                THRESHOLD_US / (" ..
    table.concat(p_execution, "*CONFIGURE_MICROSECONDS_PER_TICK ),\n                THRESHOLD_US / ( ") .. 
    "*CONFIGURE_MICROSECONDS_PER_TICK )\n" ..  "                };\n")) 
  else
    f:write("uint32_t  Execution_threshold[1+NUM_PERIODIC_TASKS] = { 0 };\n");
  end

  f:write("uint32_t  Tick_Count[1+NUM_TASKS]           = { 0")
  if #p_execution > 0 then
    f:write(string.format(", %s", table.concat(p_execution, ", ")))
  end
  if #a_execution > 0 then
    f:write(string.format(", %s", table.concat(a_execution, ", ")))
  end
  f:write(" };\n")
  
  f:write("uint32_t  Execution[1+NUM_TASKS]           = { 0")
  if #p_execution > 0 then
    f:write(string.format(", %s", table.concat(p_execution, ", ")))
  end
  if #a_execution > 0 then
    f:write(string.format(", %s", table.concat(a_execution, ", ")))
  end
  f:write(" };\n")

  f:write(string.format("%s", 
    "uint32_t  Execution_us[1+NUM_TASKS]        = {\n" ..
    "                                             " ..
    "0*CONFIGURE_MICROSECONDS_PER_TICK,\n"))

  if #p_execution > 0 then
    f:write(string.format("%s",
    "                                             " ..
    table.concat(p_execution, "*CONFIGURE_MICROSECONDS_PER_TICK,\n                                             ") ..
    "*CONFIGURE_MICROSECONDS_PER_TICK,\n"))
  end
  if #a_execution > 0 then
    f:write(string.format("%s",
    "                                             " ..
    table.concat(a_execution, "*CONFIGURE_MICROSECONDS_PER_TICK,\n                                             ") ..
    "*CONFIGURE_MICROSECONDS_PER_TICK\n"))
  end
  f:write(string.format("%s",
    "                                             };\n"))
  
  f:write("\n")

  local p_phase = get_release_times(ptasks);
  local a_release = get_release_times(atasks);
  f:write("uint32_t  Phases[1+NUM_TASKS]           = { 0")
  if #p_phase > 0 then
    f:write(string.format(", %s", table.concat(p_phase, ", ")))
  end
  if #a_release > 0 then
    f:write(string.format(", %s", table.concat(a_release, ", ")))
  end
  f:write(" };\n")
  f:write(string.format("%s", 
    "uint32_t  Phases_us[1+NUM_TASKS]        = {\n" ..
    "                                             " ..
    "0*CONFIGURE_MICROSECONDS_PER_TICK,\n"))

  if #p_phase > 0 then
    f:write(string.format("%s",
    "                                             " ..
    table.concat(p_phase, "*CONFIGURE_MICROSECONDS_PER_TICK,\n                                             ") ..
    "*CONFIGURE_MICROSECONDS_PER_TICK,\n"))
  end
  if #a_release > 0 then
    f:write(string.format("%s",
    "                                             " ..
    table.concat(a_release, "*CONFIGURE_MICROSECONDS_PER_TICK,\n                                             ") ..
    "*CONFIGURE_MICROSECONDS_PER_TICK\n"))
  end
  f:write(string.format("%s",
    "                                             };\n"))
  
  f:write("\n")

  f:write(string.format("#define build_task_name() do { \\\n"))
  local i = 1
  for _, _ in ipairs(ptasks) do
    f:write(string.format(
    "Task_name[ %d ] =  rtems_build_name( 'P', 'T', '%d', ' ' );\\\n", i,i))
    i = i + 1
  end
  for _, _ in ipairs(atasks) do
    f:write(string.format(
    "Task_name[ %d ] =  rtems_build_name( 'A', 'T', '%d', ' ' );\\\n", i,i))
    i = i + 1
  end
  f:write(string.format("} while(0)\n"))

  f:write("\n")
  f:write("#endif\n")

  f:flush()
  f:close()
end

-- main entry point
if not arg[1] then
  usage(arg)
  return
end

-- extract task descriptions from arguments
tasks = parse_args(arg)

-- sort task descriptions
table.sort(tasks, task_lt)

-- extract periodic and aperiodic descriptions
periodic_tasks = get_periodic_tasks(tasks)
aperiodic_tasks = get_aperiodic_tasks(tasks)

-- extract deadlines / periods
periods = get_deadlines(periodic_tasks)

-- determine length of a hyperperiod
hyperperiod_length = get_hyperperiod_length(periods)

-- determine set of jobs
J = get_jobs(periodic_tasks, hyperperiod_length)

-- J is a sorted table of tables, where the table at 
-- J[i], i = 1 to n, has entries:
--  J[i][1] = deadline of job i
--  J[i][2] = execution time of job i
--  J[i][3] = release time of job i
-- J is ordered by earliest deadline, with ties broken by earliest release.

-- compute \sigma_i(0) for i = 1 to n, the initial slack of all jobs
compute_initial_slacks(J)

-- compute the slack table
slack_table = compute_slack_table(J, hyperperiod_length)

write_header(aperiodic_tasks, periodic_tasks, J, slack_table, hyperperiod_length)

-- now generate the headers without the slack table.
write_header(aperiodic_tasks, periodic_tasks, J, nil, hyperperiod_length)
