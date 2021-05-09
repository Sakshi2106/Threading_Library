

<br />
<p align="center">
  <h1 align="center">Threading_Library</h1>
  <p align="center">
    <h3 align="center">A multi-threading library for C Programs</h3>
    <!-- <a href="https://github.com/othneildrew/Best-README-Template">View Demo</a> -->
    <!-- <p align="center">
    <a href="https://gitlab.com/andipro123/cthread/-/issues">Report Bug</a>
    ·
    <a href="https://gitlab.com/andipro123/cthread/-/issues">Request Feature</a>
    </p>
  </p>
</p> -->



<!-- TABLE OF CONTENTS -->
<!-- <details open="open">
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#one-one">One-One</a></li>
        <li><a href="#many-one">Many-One</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#testing">Testing</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#acknowledgements">Acknowledgements</a></li>
    <li><a href="#authors">Authors</a></li>
  </ol>
</details> -->



<!-- ABOUT THE PROJECT -->
## About The Project

Threading_Library is a multi-threading library which implements POSIX compliant threads in the Linux userland.  
Threading_Library consists of two models. They can be classified depending on how user threads are mapped to
kernel threads. Two models are:

* **One-One**

    One-One model maps each user-level thread with a kernel-level thread.  
    In this model, thread management is done at the kernel level and the user threads are fully supported by the kernel.
    Here, if one thread makes the blocking system call, the other threads still run.
    <img src="https://binaryterms.com/wp-content/uploads/2019/07/One-to-one-Multithreading-Model.jpg" alt="one-one">
    <br></br>

* **Many-One**

    Many-One model maps many  user-level threads to a single kernel-level thread. 
    The thread library present at the user space is responsible for thread management at the user level.   

    <img src="https://binaryterms.com/wp-content/uploads/2019/07/Many-to-one-Multithreading-Model.jpg" alt="many-one">




## Testing

The library comes with an automatic testing package to test all the functionalities of the threading library. Tests cover all areas of multi-threading to ensure that  basic functions are working as expected. To run the tests:

```bash 
#compile and auto run
$ make run
```

<!-- ACKNOWLEDGEMENTS -->
## References
* [Implementing Threads](http://www.it.uu.se/education/course/homepage/os/vt18/module-4/implementing-threads/#kernel-level-threads)
* [Signal Handling](https://www.gnu.org/software/libc/manual/html_node/Signal-Handling.html)
* [Introduction to Project](https://sites.cs.ucsb.edu/~chris/teaching/cs170/projects/proj2.html)


## Author

- [@Sakshi2106](https://github.com/Sakshi2106)


