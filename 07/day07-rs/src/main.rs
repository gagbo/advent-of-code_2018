use std::cell::RefCell;
use std::collections::HashMap;
use std::env;
use std::fs::File;
use std::io;
use std::io::BufRead;
use std::rc::Rc;

extern crate time;
#[macro_use]
extern crate text_io;

fn main() {
    let args = env::args().collect::<Vec<String>>();
    if args.len() < 3 {
        panic!("Not enough arguments")
    }

    let input_filename = &args[2];
    let input_lines = read_input(input_filename).unwrap();

    let start_t = time::precise_time_s();
    let result = match args[1].as_str() {
        "a" => {
            println!("Calling Part A");
            part_a(input_lines)
        }
        "b" => {
            println!("Calling Part B");
            part_b(input_lines, 5, 60)
        }
        _ => panic!("Expecting a or b as 1st argument"),
    };
    let end_t = time::precise_time_s();
    println!("{} : in {} seconds", result, end_t - start_t);
}

fn read_input(filename: &str) -> Result<io::BufReader<File>, io::Error> {
    let file = File::open(filename)?;
    let buf_reader = io::BufReader::new(file);
    Ok(buf_reader)
}

fn part_a(input_lines: io::BufReader<File>) -> String {
    let mut list = TaskList::new();
    for line in input_lines.lines() {
        let line = line.unwrap();
        let id_caller: char;
        let id_needed: char;
        scan!(line.bytes() => "Step {} must be finished before step {} can begin.", id_needed, id_caller);
        list.add_dependency(id_caller, id_needed, 0);
    }
    list.print();
    list.pop_until_done()
}

fn part_b(input_lines: io::BufReader<File>, worker_count: usize, time_offset: usize) -> String {
    let mut list = TaskList::new();
    for line in input_lines.lines() {
        let line = line.unwrap();
        let id_caller: char;
        let id_needed: char;
        scan!(line.bytes() => "Step {} must be finished before step {} can begin.", id_needed, id_caller);
        list.add_dependency(id_caller, id_needed, time_offset);
    }
    list.work_until_done(worker_count).to_string()
}

struct Task {
    id: char,
    time_to_complete: usize,
    undertaken: bool,
    prereq: HashMap<char, Rc<RefCell<Task>>>,
}

struct TaskList {
    list: HashMap<char, Rc<RefCell<Task>>>,
}

impl TaskList {
    pub fn new() -> Self {
        TaskList {
            list: HashMap::new(),
        }
    }

    pub fn add_dependency(&mut self, id_caller: char, id_needed: char, time_offset: usize) {
        let entry_caller = self.entry(id_caller, time_offset);
        let entry_needed = self.entry(id_needed, time_offset);
        entry_caller
            .borrow_mut()
            .prereq
            .insert(id_needed, entry_needed.clone());
    }

    pub fn entry(&mut self, id_task: char, time_offset: usize) -> Rc<RefCell<Task>> {
        self.list
            .entry(id_task)
            .or_insert(Rc::new(RefCell::new(Task::new(id_task, time_offset))))
            .clone()
    }

    pub fn print(&self) {
        for val in self.list.values() {
            println!(
                "Task {} : {} deps",
                val.borrow().id,
                val.borrow().prereq.len()
            );
        }
    }

    pub fn remove_task(&mut self, id_done: char) {
        match self.list.remove(&id_done) {
            Some(_) => {
                self.remove_dependency(id_done);
            }
            None => {
                println!(
                    "Tried to remove {} but no such task exists in the list",
                    id_done
                );
            }
        };
    }

    pub fn remove_dependency(&mut self, id_done: char) {
        for task in self.list.values() {
            task.borrow_mut().prereq.remove(&id_done);
        }
    }

    pub fn check_available_task(&self) -> bool {
        self.list
            .iter()
            .filter(|(_, v)| v.borrow().prereq.len() == 0)
            .any(|(_, v)| !v.borrow().undertaken)
    }

    pub fn pick_available_task(&mut self) -> Rc<RefCell<Task>> {
        let (id_task, candidate) = self
            .list
            .iter()
            .filter(|(_, v)| v.borrow().prereq.len() == 0 && !v.borrow().undertaken)
            .min_by(|(c, _), (d, _)| c.cmp(&d))
            .unwrap();
        self.mark_task(id_task.clone());
        let result = candidate.clone();
        println!("Picking up {} ", id_task);
        result
    }

    pub fn mark_task(&self, id_taking: char) {
        let task = self.list.get(&id_taking);
        task.unwrap().borrow_mut().undertaken = true;
    }

    pub fn pop_task(&mut self) -> char {
        let (id_candidate, _candidate) = self
            .list
            .iter()
            .filter(|(_, v)| v.borrow().prereq.len() == 0)
            .min_by(|(c, _), (d, _)| c.cmp(&d))
            .unwrap();
        let result = id_candidate.clone();
        println!("Candidate is : {} ", result);
        self.remove_task(result);
        result
    }

    pub fn pop_until_done(&mut self) -> String {
        let mut result: String = String::from("");
        loop {
            if self.list.len() == 0 {
                break;
            }
            result.push(self.pop_task());
        }
        result
    }

    pub fn work_until_done(&mut self, worker_count: usize) -> isize {
        let mut duration = 0;
        let mut time_step = 1;
        let mut minimal_time_step: bool;
        // Workers either have nothing to do or is currently working a task
        let mut workers: Vec<Option<Rc<RefCell<Task>>>> = vec![None; worker_count];
        loop {
            println!("######################");
            println!("{:022}", duration);
            println!("######################");
            if self.list.len() == 0 {
                return duration;
            }
            minimal_time_step = false;
            let mut finished_jobs: Vec<char> = Vec::new();

            for worker in &mut workers {
                match worker {
                    // Lazy workers check if there is work to pick up
                    None => {
                        println!("Checking for a job");
                        if self.check_available_task() {
                            let task = self.pick_available_task();
                            task.borrow_mut().update_timer(1);
                            *worker = Some(task);
                        }
                    }
                    // Other workers check if they should notice the tasklist they're done
                    Some(task) => {
                        if task.borrow_mut().is_done_in_time(time_step) {
                            let id_task = task.borrow_mut().id.clone();
                            *worker = None;
                            println!("Finished {}", id_task);
                            finished_jobs.push(id_task);

                            // Force the time_step to be one second so the new state of jobs is
                            // checked as quickly as possible.

                            // XXX : won't work if there's a job lasting only 1 second and other
                            // jobs finishing at the same second. The bool forces the time-step to
                            // be 1, but the shortest time-step should be 0 (just to register the
                            // end of the 1 second job). The solution would probably be to handle
                            // properly the 1 second job case to avoid having the 0s time-step
                            // corner case.
                            minimal_time_step = true;
                        } else {
                            task.borrow_mut().update_timer(time_step);
                        }
                    }
                }
            }

            for id_task in finished_jobs {
                self.remove_task(id_task);
            }

            println!(
                "{} jobs remaining, {} active workers",
                self.list.len(),
                workers.iter().flatten().count()
            );

            if minimal_time_step {
                time_step = 1;
            } else {
                time_step = workers
                    .iter()
                    .flatten()
                    .map(|task| task.borrow().time_to_complete)
                    .min()
                    .unwrap();
            }
            println!("Time-step is {}", time_step);
            duration += time_step as isize;
        }
    }
}

impl Task {
    pub fn new(id: char, time_offset: usize) -> Self {
        Task {
            id,
            time_to_complete: time_offset + (id as usize - 'A' as usize + 1),
            undertaken: false,
            prereq: HashMap::new(),
        }
    }

    pub fn update_timer(&mut self, duration: usize) {
        if duration > self.time_to_complete {
            panic!("Trying to go past the time remaining to complete the task !");
        }
        self.time_to_complete -= duration;
    }

    pub fn is_done_in_time(&mut self, duration: usize) -> bool {
        duration >= self.time_to_complete
    }
}

#[cfg(test)]
#[test]
pub fn example_part_a_from_aoc() {
    let input_lines = read_input("../test_input.txt").unwrap();
    assert_eq!(part_a(input_lines), "CABDFE");
}

#[cfg(test)]
#[test]
pub fn answer_part_a_from_aoc() {
    let input_lines = read_input("../input.txt").unwrap();
    assert_eq!(part_a(input_lines), "JMQZELVYXTIGPHFNSOADKWBRUC");
}

#[cfg(test)]
#[test]
pub fn example_part_b_from_aoc() {
    let input_lines = read_input("../test_input.txt").unwrap();
    assert_eq!(part_b(input_lines, 2, 0), "15");
}

#[cfg(test)]
#[test]
pub fn answer_part_b_from_aoc() {
    let input_lines = read_input("../input.txt").unwrap();
    assert_eq!(part_b(input_lines, 5, 60), "1133");
}
