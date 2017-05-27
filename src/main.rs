use std::error::Error;
use std::env;
use std::fs::File;
use std::path::Path;
use std::io::prelude::*;
use std::process::exit;

struct Cell {
    fixed: bool,
    val: i8,
}

struct Grid {
    size: i8,
    block_size: i8,
    data: Vec<Vec<Cell>>,
}

fn main() {
    let args: Vec<String> = env::args().collect();

    if args.len() != 2 {
        println!("usage: sudoku FILE");
        exit(1);
    }

    let path = Path::new(&args[1]);
    let display = path.display();

    let mut file = match File::open(&path) {
        Err(why) => panic!("couldn't open {}: {}", display, why.description()),
        Ok(file) => file,
    };
}
