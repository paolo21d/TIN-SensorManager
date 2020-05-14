package tin.administrator.controller;

import javafx.concurrent.Task;

public class LabelRefresher extends Task<String> {
    private String content;
    @Override
    protected String call() throws Exception {
        synchronized (this){
            wait();
            while(true){
                updateMessage(content);
                wait();
            }
        }
    }

    public void refresh(String text){
        synchronized (this){
            content = text;
            notify();
        }
    }
}
