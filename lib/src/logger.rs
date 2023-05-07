use chrono;
use fern;
use fern::colors::{Color, ColoredLevelConfig};
use log::Level;

pub fn init_logging() -> Result<(), fern::InitError> {
    let mut colors = ColoredLevelConfig::new();

    colors.error = Color::Red;
    colors.warn = Color::Yellow;
    colors.info = Color::Green;
    colors.debug = Color::Blue;
    colors.trace = Color::Magenta;

    let start = chrono::Local::now();

    let logger = fern::Dispatch::new()
        .filter(|metadata| metadata.level() <= Level::Info)
        .format(move |out, message, record| {
            out.finish(format_args!(
                "{timestamp} ::: {module} [{level}] > {message}",
                timestamp = {
                    let duration = chrono::Local::now() - start;

                    let hours = duration.num_hours();
                    let duration = duration
                        .checked_sub(&chrono::Duration::hours(hours))
                        .unwrap();

                    let minutes = duration.num_minutes();
                    let duration = duration
                        .checked_sub(&chrono::Duration::minutes(minutes))
                        .unwrap();

                    let seconds = duration.num_seconds();
                    let duration = duration
                        .checked_sub(&chrono::Duration::seconds(seconds))
                        .unwrap();

                    let micros = duration.num_nanoseconds().unwrap();
                    format!("{}:{:02}:{:02}.{:09}", hours, minutes, seconds, micros)
                },
                module = record.target(),
                level = colors.color(record.level()),
                message = message
            ))
        });

    // if env::consts::OS == "macos" {
    //     logger.chain(std::io::stdout()).apply()?;
    // } else {
    //     logger.chain(std::io::stdout()).apply()?;
    // }
    logger.chain(std::io::stdout()).apply()?;

    Ok(())
}
