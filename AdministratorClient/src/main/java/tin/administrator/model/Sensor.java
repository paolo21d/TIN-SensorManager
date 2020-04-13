package tin.administrator.model;

import lombok.AllArgsConstructor;
import lombok.Data;

@Data
@AllArgsConstructor
public class Sensor {
    private long id;
    private String name;
    private String ip;
    private Long port;

}
