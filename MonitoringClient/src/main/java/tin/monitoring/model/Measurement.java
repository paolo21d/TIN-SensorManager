package tin.monitoring.model;

import lombok.AllArgsConstructor;
import lombok.Data;

@Data
@AllArgsConstructor
public class Measurement {

    private int id;
    private int value;
    private String label;
}
