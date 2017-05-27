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

/// Read a grid in from a named file.
///
/// The given file should contain an NxN grid of integers or '.' for cells that
/// are initially empty, where N is a perfect square (typically 9, but any
/// square is supported). If any of these conditions are violated `Err` is
/// returned with an appropriate error message, otherwise `Ok` is returned
/// containing the `Grid` that corresponds to the given file.
fn read_grid(path: &Path) -> Result<Grid, String> {
    let display = path.display();

    let mut file = match File::open(&path) {
        Err(why) => return Err(format!("couldn't open {}: {}", display,
                                       why.description())),
        Ok(file) => file,
    };

    let mut contents = String::new();
    file.read_to_string(&mut contents);

    let mut data = Vec::new();
    for line in contents.lines() {
        let mut row = Vec::new();
        for cell in line.split_whitespace() {
            if cell == "." {
                row.push(Cell { fixed: false, val: 0 });
            } else {
                match cell.parse::<i8>() {
                    Err(_) => return Err(format!("bad cell: {}", cell)),
                    Ok(n) => row.push(Cell { fixed: true, val: n }),
                }
            }
        }

        let rowlen = row.len();
        data.push(row);
        if rowlen != data[0].len() {
            return Err(format!("bad Sudoku file: {}", display));
        }
    }

    let size = data[0].len() as i8;
    let height = data.len() as i8;
    if size != height {
        return Err(format!("bad Sudoku dimensions: {}x{}", height, size));
    }

    let size_root = (size as f64).sqrt() as i8;
    if size_root.pow(2) != size {
        return Err(format!("bad Sudoku dimensions: {}x{}", size, size));
    }

    Ok(Grid { size: size, block_size: size_root as i8, data: data })
}

fn main() {
    let args: Vec<String> = env::args().collect();

    if args.len() != 2 {
        println!("usage: sudoku FILE");
        exit(1);
    }

    let path = Path::new(&args[1]);
    let grid = match read_grid(&path) {
        Err(why) => panic!(why),
        Ok(g) => g,
    };
}
