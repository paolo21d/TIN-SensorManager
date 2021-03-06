package tin.monitoring.model;

import lombok.AllArgsConstructor;
import lombok.Data;

@Data
@AllArgsConstructor
public class Sensor {
    private Integer id;
    private String name;
    private String ip;
    private Integer port;
    private Integer currentMeasurement;
}
