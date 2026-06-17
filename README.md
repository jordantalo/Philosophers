*This project has been created as part of the 42 curriculum by jtalobre.*

# Philosophers

## Description
**Philosophers** is a 42 school curriculum project designed as a practical introduction to the fundamentals of concurrent programming. Based on Edsger Dijkstra's famous *Dining Philosophers problem*, the project explores the challenges of synchronization, shared memory, and resource allocation.

The core objective is to create a simulation where a group of philosophers sit around a round table with a bowl of spaghetti. They alternate between three states: **eating**, **sleeping**, and **thinking**. However, there are only as many forks as there are philosophers, and a philosopher needs **two forks** to eat. The challenge is to design an architecture that prevents data races, ensures proper timing, and completely avoids **deadlocks** (where everyone holds one fork and starves) or **starvation**.

---

## The rules & arguments

### Input format
The executable accepts the following configuration parameters (all times are in milliseconds):

```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```
* number_of_philosophers: The total number of philosophers sitting at the table, which also equals the total number of forks available.

* time_to_die: If a philosopher does not start eating within this timeframe since the beginning of their last meal (or the start of the simulation), they die.

* time_to_eat: The duration it takes for a philosopher to eat. They must hold two forks during this entire process.

* time_to_sleep: The duration a philosopher spends sleeping after completing their meal.

* number_of_times_each_philosopher_must_eat (Optional): If specified, the simulation stops successfully once every philosopher has eaten at least this many times. If omitted, the simulation runs indefinitely until a philosopher dies.

### Table layout
* Philosophers are numbered from 1 to number_of_philosophers.

* They sit in a circle: Philosopher 1 sits next to the last philosopher. Any philosopher N sits between N - 1 and N + 1.

## Technical architecture and implementations

This project is divided into two distinct architectures depending on the version implemented:

### Mandatory part: thread & mutexes

The mandatory version focuses on standard multi-threading inside a single process.

* Threads: The program spawns exactly one thread per philosopher. Each thread runs an independent simulation routine loop (Take forks → Eat → Sleep → Think).

* Mutexes: Forks are represented as standard mutexes (pthread_mutex_t). To prevent deadlocks, an asymmetric picking order is applied: even-numbered philosophers and odd-numbered philosophers pick up their left/right forks in alternating sequences (e.g., Even: Left then Right; Odd: Right then Left).

* Shared State: As philosophers eat, they update internal shared metrics (such as the timestamp of their last meal and their meal count) using guarded mutex writes to prevent data races.

* Global Monitor: A separate, independent monitoring thread runs continuously in the background. It iterates through all philosophers, checking their last meal time against time_to_die, or verifying if the optional meal satisfaction quota has been reached. If a death occurs, the monitor stops the simulation immediately.

### Bonus part: processes & semaphores

The bonus version completely restructures the simulation using separate process environments.

* Processes: Instead of threads, the main program uses fork() to spawn an isolated child process for each individual philosopher.

* Kernel Semaphores: Forks are no longer tied to individual seating spaces. Instead, they are represented globally by a single, shared Linux-kernel named semaphore (sem_t). All available forks reside in this centralized pool.

* Dual-Threaded Child Routing: Each child process runs exactly two threads:

	* The primary thread handles the standard eating/sleeping/thinking loop routine.

	* A local monitoring thread constantly watches only that specific philosopher to check if they have exceeded their time_to_die.

* Global Shutdown Mechanics: If a local monitor detects a death, it reports the state by posting to a dedicated termination semaphore. The parent process, which is blocking and listening on this specific semaphore, instantly catches the signal and broadcasts a termination kill signal to all remaining child processes, safely cleaning up the system state.

## Instructions

### Compilation

Compile the mandatory or bonus project using the provided Makefile:

```bash
git clone https://github.com/jordantalo/Philosophers.git philosophers
cd philosophers
```

then move to the mandatory or bonus project (cd philo or cd philo_bonus)

```bash
make
```

### Execution examples

Run a safe simulation where 4 philosophers alternate perfectly without dying:

```bash
./philo 4 410 200 200
```

Run a simulation where a philosopher is bound to starve (Die condition test):
```bash
./philo 1 800 200 200
```

Run a simulation that terminates automatically after everyone has eaten at least 5 times:
```bash
./philo 5 800 200 200 5
```

## Ressources

* [Dining philosophers problem](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
* [Dining Philosopher Solution using Semaphores](https://www.geeksforgeeks.org/operating-systems/dining-philosopher-problem-using-semaphores/)
* [Philosophers 42 Guide](https://medium.com/@ruinadd/philosophers-42-guide-the-dining-philosophers-problem-893a24bc0fe2)
* [Unix thread in c](https://www.youtube.com/watch?v=d9s_d28yJq0&list=PLfqABt5AS4FmuQf70psXrsMLEDQXNkLq2)


