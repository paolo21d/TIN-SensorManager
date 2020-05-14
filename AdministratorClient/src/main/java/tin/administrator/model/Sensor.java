package tin.administrator.model;

import lombok.AllArgsConstructor;
import lombok.Data;

@Data
@AllArgsConstructor
public class Sensor {
    private Integer id;
    private String name;
    private String ip;
    private Integer port;
    private Boolean connected;
}
