package com.example.sthome;

public class ThreadUtil {

    // Return thread info data include thread id, name and priority.
    public static String getThreadInfo(Thread thread)
    {
        long threadId = thread.getId();
        String threadName = thread.getName();
        int threadPriority = thread.getPriority();

        StringBuffer buffer = new StringBuffer();
        buffer.append(" id = ");
        buffer.append(threadId);
        buffer.append(" , name = ");
        buffer.append(threadName);
        buffer.append(" , priority = ");
        buffer.append(threadPriority);

        return buffer.toString();
    }
}